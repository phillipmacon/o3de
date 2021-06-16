/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

//     Got rid of unzip usage, now using ZipDir for much more effective
//     memory usage (~3-6 times less memory, and no allocator overhead)
//     to keep the directory of the zip file; better overall effectiveness and
//     more readable and manageable code, made the connection to Streaming Engine


#pragma once


#include <AzCore/IO/CompressionBus.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/std/parallel/mutex.h>
#include <AzCore/std/parallel/lock.h>
#include <AzCore/std/parallel/thread.h>
#include <AzCore/std/smart_ptr/intrusive_base.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <AzCore/std/string/fixed_string.h>
#include <AzCore/std/string/osstring.h>

#include <AzFramework/Archive/IArchive.h>
#include <AzFramework/Archive/ZipDirCache.h>

namespace AzFramework
{
    class AssetBundleManifest;
    class AssetRegistry;
}

namespace AZ::IO
{
    class Archive;
    // this is the header in the cache of the file data
    struct CCachedFileData
        : public AZStd::intrusive_base
    {
        AZ_CLASS_ALLOCATOR(CCachedFileData, AZ::SystemAllocator, 0);
        CCachedFileData(ZipDir::CachePtr pZip, uint32_t nArchiveFlags, ZipDir::FileEntry* pFileEntry, AZStd::string_view szFilename);
        ~CCachedFileData();
        CCachedFileData(const CCachedFileData&) = delete;
        CCachedFileData& operator=(const CCachedFileData&) = delete;

        // return the data in the file, or nullptr if error
        // by default, if bRefreshCache is true, and the data isn't in the cache already,
        // the cache is refreshed. Otherwise, it returns whatever cache is (nullptr if the data isn't cached yet)
        // decompress can be harmlessly set to true if you want the data back decompressed.
        // set them to false only if you want to operate on the raw data while its still compressed.
        void* GetData(bool bRefreshCache = true, bool decompress = true);
        // Uncompress file data directly to provided memory.
        bool GetDataTo(void* pFileData, int nDataSize, bool bDecompress = true);

        // Return number of copied bytes, or -1 if did not read anything
        int64_t ReadData(void* pBuffer, int64_t nFileOffset, int64_t nReadSize);

        ZipDir::Cache* GetZip() { return m_pZip.get(); }
        ZipDir::FileEntry* GetFileEntry() { return m_pFileEntry; }

        uint32_t GetFileDataOffset();

        void* m_pFileData;

        // the zip file in which this file is opened
        ZipDir::CachePtr m_pZip;
        uint32_t m_nArchiveFlags;
        // the file entry : if this is nullptr, the entry is free and all the other fields are meaningless
        ZipDir::FileEntry* m_pFileEntry;
    };

    using CCachedFileDataPtr = AZStd::intrusive_ptr<CCachedFileData>;

    namespace ArchiveInternal
    {
        struct CCachedFileRawData;
        struct CZipPseudoFile;
    };

    //////////////////////////////////////////////////////////////////////
    class Archive
        : public IArchive
        , public AZ::IO::CompressionBus::Handler
    {
    public:
        AZ_RTTI(Archive, "{764A2260-FF8A-4C86-B958-EBB0B69D9DFA}", IArchive);
        AZ_CLASS_ALLOCATOR(Archive, AZ::OSAllocator, 0);
    private:
        friend struct CCachedFileData;
        friend class FindData;
        friend class NestedArchive;
        friend struct SAutoCollectFileAccessTime;

        // the array of pseudo-files : emulated files in the virtual zip file system
        // the handle to the file is its index inside this array.
        // some of the entries can be free. The entries need to be destructed manually
        using ZipPseudoFileArray = AZStd::vector<AZStd::unique_ptr<ArchiveInternal::CZipPseudoFile>, AZ::OSStdAllocator>;

        // This is a cached data for the FGetCachedFileData call.
        struct CachedRawDataEntry;
        using CachedFileRawDataSet = AZStd::unordered_map<AZ::IO::HandleType, CachedRawDataEntry, AZStd::hash<AZ::IO::HandleType>, AZStd::equal_to<>, AZ::OSStdAllocator>;

        // open zip cache objects that can be reused. They're self-[un]registered
        // they're sorted by the path to archive file
        using ArchiveArray = AZStd::vector<INestedArchive*, AZ::OSStdAllocator>;

        // the array of opened caches - they get destructed by themselves (these are intrusive, see the ZipDir::Cache documentation)
        struct PackDesc
        {
            AZ::IO::Path m_pathBindRoot; // the zip binding root
            AZStd::string strFileName; // the zip file name (with path) - very useful for debugging so please don't remove

            // [LYN-2376] Remove once legacy slice support is removed
            bool m_containsLevelPak = false; // indicates whether this archive has level.pak inside it or not  

            const char* GetFullPath() const { return pZip->GetFilePath(); }

            AZStd::intrusive_ptr<INestedArchive> pArchive;
            ZipDir::CachePtr pZip;
        };
        using ZipArray = AZStd::vector<PackDesc, AZ::OSStdAllocator>;

        // ArchiveFindDataSet entire purpose is to keep a reference to the intrusive_ptr of ArchiveFindData
        // so that it doesn't go out of scope
        using ArchiveFindDataSet = AZStd::set<AZStd::intrusive_ptr<AZ::IO::FindData>, AZ::OSStdAllocator>;

        // given the source relative path, constructs the full path to the file according to the flags
        const char* AdjustFileName(AZStd::string_view src, char* dst, size_t dstSize, uint32_t nFlags, bool skipMods = false) override;


        /**
         * Currently access to PseudoFile operations are not thread safe, as we touch variable like m_nCurSeek
         * without any synchronization. There is also the assumption that only one thread at a time will open/read/close
         * a single file in a PAK, multiple threads can open different files in a PAK. If requirements change (or turns out this is a bug in Archive)
         * we can add readwrite lock inside the CZipPseudoFile and we can pass a second argument lockForOperation where we can lock
         * the file specific lock while getting a handle. This way you can safely execute the operation. This is no done by default
         * because the API implies that this is not the intended use case. e.g. FSeek and FRead are separate operations, if you have multiple threads
         * reading data, they can both execute FSeek/FRead and unless we lock the operation set, this will not work.
         */
        ArchiveInternal::CZipPseudoFile* GetPseudoFile(AZ::IO::HandleType fileHandle) const;

    public:

        Archive();
        ~Archive();

        //! CompressionBus Handler implementation.
        void FindCompressionInfo(bool& found, AZ::IO::CompressionInfo& info, const AZStd::string_view filename) override;

        //! Processes an alias command line containing multiple aliases.
        void ParseAliases(AZStd::string_view szCommandLine) override;
        //! adds or removes an alias from the list - if bAdd set to false will remove it
        void SetAlias(AZStd::string_view szName, AZStd::string_view szAlias, bool bAdd) override;
        //! gets an alias from the list, if any exist.
        //! if bReturnSame==true, it will return the input name if an alias doesn't exist. Otherwise returns nullptr
        const char* GetAlias(AZStd::string_view szName, bool bReturnSame = true) override;

        // Set the localization folder
        void SetLocalizationFolder(AZStd::string_view sLocalizationFolder) override;
        const char* GetLocalizationFolder() const override { return m_sLocalizationFolder.c_str(); }
        const char* GetLocalizationRoot() const override { return m_sLocalizationRoot.c_str(); }

        // lock all the operations
        void Lock() override;
        void Unlock() override;

        // open the physical archive file - creates if it doesn't exist
        // returns nullptr if it's invalid or can't open the file
        AZStd::intrusive_ptr<INestedArchive> OpenArchive(AZStd::string_view szPath, AZStd::string_view bindRoot = {}, uint32_t nFlags = 0, AZStd::intrusive_ptr<AZ::IO::MemoryBlock> pData = nullptr) override;

        // returns the path to the archive in which the file was opened
        const char* GetFileArchivePath(AZ::IO::HandleType fileHandle) override;

        //////////////////////////////////////////////////////////////////////////

        //! Return pointer to pool if available
        void* PoolMalloc(size_t size) override;
        //! Free pool
        void PoolFree(void* p) override;

        AZStd::intrusive_ptr<AZ::IO::MemoryBlock> PoolAllocMemoryBlock(size_t nSize, const char* sUsage, size_t nAlign) override;

        // interface IArchive ---------------------------------------------------------------------------

        void RegisterFileAccessSink(IArchiveFileAccessSink* pSink) override;
        void UnregisterFileAccessSink(IArchiveFileAccessSink* pSink) override;

        bool Init(AZStd::string_view szBasePath) override;
        void Release() override;

        bool IsInstalledToHDD(AZStd::string_view acFilePath = 0) const override;

        // [LYN-2376] Remove 'addLevels' parameter once legacy slice support is removed
        bool OpenPack(AZStd::string_view pName, uint32_t nFlags = 0, AZStd::intrusive_ptr<AZ::IO::MemoryBlock> pData = nullptr, AZ::IO::FixedMaxPathString* pFullPath = nullptr, bool addLevels = true) override;
        bool OpenPack(AZStd::string_view szBindRoot, AZStd::string_view pName, uint32_t nFlags = 0, AZStd::intrusive_ptr<AZ::IO::MemoryBlock> pData = nullptr, AZ::IO::FixedMaxPathString* pFullPath = nullptr, bool addLevels = true) override;
        // after this call, the file will be unlocked and closed, and its contents won't be used to search for files
        bool ClosePack(AZStd::string_view pName, uint32_t nFlags = 0) override;
        bool OpenPacks(AZStd::string_view pWildcard, uint32_t nFlags = 0, AZStd::vector<AZ::IO::FixedMaxPathString>* pFullPaths = nullptr) override;
        bool OpenPacks(AZStd::string_view szBindRoot, AZStd::string_view pWildcard, uint32_t nFlags = 0, AZStd::vector<AZ::IO::FixedMaxPathString>* pFullPaths = nullptr) override;

        // closes pack files by the path and wildcard
        bool ClosePacks(AZStd::string_view pWildcard, uint32_t nFlags = 0) override;

        //returns if a archive exists matching the wildcard
        bool FindPacks(AZStd::string_view pWildcardIn) override;

        // prevent access to specific archive files
        bool SetPacksAccessible(bool bAccessible, AZStd::string_view pWildcard, uint32_t nFlags = 0) override;
        bool SetPackAccessible(bool bAccessible, AZStd::string_view pName, uint32_t nFlags = 0) override;

        // returns the file modification time
        uint64_t GetModificationTime(AZ::IO::HandleType fileHandle) override;

        bool LoadPakToMemory(AZStd::string_view pName, EInMemoryArchiveLocation nLoadArchiveToMemory, AZStd::intrusive_ptr<AZ::IO::MemoryBlock> pMemoryBlock = nullptr) override;
        void LoadPaksToMemory(int nMaxArchiveSize, bool bLoadToMemory) override;

        AZ::IO::HandleType FOpen(AZStd::string_view pName, const char* mode, uint32_t nPathFlags = 0) override;
        size_t FReadRaw(void* data, size_t length, size_t elems, AZ::IO::HandleType handle) override;
        size_t FReadRawAll(void* data, size_t nFileSize, AZ::IO::HandleType handle) override;
        void* FGetCachedFileData(AZ::IO::HandleType handle, size_t& nFileSize) override;
        size_t FWrite(const void* data, size_t length, size_t elems, AZ::IO::HandleType handle) override;
        size_t FSeek(AZ::IO::HandleType handle, uint64_t seek, int mode) override;
        uint64_t FTell(AZ::IO::HandleType handle) override;
        int FFlush(AZ::IO::HandleType handle) override;
        int FClose(AZ::IO::HandleType handle) override;
        AZ::IO::ArchiveFileIterator FindFirst(AZStd::string_view pDir, EFileSearchType searchType = eFileSearchType_AllowInZipsOnly) override;
        AZ::IO::ArchiveFileIterator FindNext(AZ::IO::ArchiveFileIterator fileIterator) override;
        bool FindClose(AZ::IO::ArchiveFileIterator fileIterator) override;
        int FEof(AZ::IO::HandleType handle) override;
        char* FGets(char*, int, AZ::IO::HandleType) override;
        int Getc(AZ::IO::HandleType) override;
        int FPrintf(AZ::IO::HandleType handle, const char* format, ...) override;
        size_t FGetSize(AZ::IO::HandleType fileHandle) override;
        size_t FGetSize(AZStd::string_view sFilename, bool bAllowUseFileSystem = false) override;
        bool IsInPak(AZ::IO::HandleType handle) override;
        bool RemoveFile(AZStd::string_view pName) override; // remove file from FS (if supported)
        bool RemoveDir(AZStd::string_view pName) override;  // remove directory from FS (if supported)
        bool IsAbsPath(AZStd::string_view pPath) override;

        bool IsFileExist(AZStd::string_view sFilename, EFileSearchLocation fileLocation = eFileLocation_Any) override;
        bool IsFolder(AZStd::string_view sPath) override;
        IArchive::SignedFileSize GetFileSizeOnDisk(AZStd::string_view filename) override;

        // creates a directory
        bool MakeDir(AZStd::string_view szPath, bool bGamePathMapping = false) override;

        // compresses the raw data into raw data. The buffer for compressed data itself with the heap passed. Uses method 8 (deflate)
        // returns one of the Z_* errors (Z_OK upon success)
        // MT-safe
        int RawCompress(const void* pUncompressed, size_t* pDestSize, void* pCompressed, size_t nSrcSize, int nLevel = -1) override;

        // Uncompresses raw (without wrapping) data that is compressed with method 8 (deflated) in the Zip file
        // returns one of the Z_* errors (Z_OK upon success)
        // This function just mimics the standard uncompress (with modification taken from unzReadCurrentFile)
        // with 2 differences: there are no 16-bit checks, and
        // it initializes the inflation to start without waiting for compression method byte, as this is the
        // way it's stored into zip file
        int RawUncompress(void* pUncompressed, size_t* pDestSize, const void* pCompressed, size_t nSrcSize) override;

        //////////////////////////////////////////////////////////////////////////
        // Files opening recorder.
        //////////////////////////////////////////////////////////////////////////

        void RecordFileOpen(ERecordFileOpenList eMode) override;
        ERecordFileOpenList GetRecordFileOpenList() override;
        void RecordFile(AZ::IO::HandleType in, AZStd::string_view szFilename) override;

        IResourceList* GetResourceList(ERecordFileOpenList eList) override;
        void SetResourceList(ERecordFileOpenList eList, IResourceList* pResourceList) override;

        uint32_t ComputeCRC(AZStd::string_view szPath, uint32_t nFileOpenFlags = 0) override;
        bool ComputeMD5(AZStd::string_view szPath, uint8_t* md5, uint32_t nFileOpenFlags = 0, bool useDirectFileAccess = false) override;

        void DisableRuntimeFileAccess(bool status) override
        {
            m_disableRuntimeFileAccess[0] = status;
            m_disableRuntimeFileAccess[1] = status;
        }

        bool DisableRuntimeFileAccess(bool status, AZStd::thread_id threadId) override;
        bool CheckFileAccessDisabled(AZStd::string_view name, const char* mode) override;

        void SetRenderThreadId(AZStd::thread_id renderThreadId) override
        {
            m_renderThreadId = renderThreadId;
        }

        // gets the current archive priority
        ArchiveLocationPriority GetPakPriority() const override;

        uint64_t GetFileOffsetOnMedia(AZStd::string_view szName) const override;

        EStreamSourceMediaType GetFileMediaType(AZStd::string_view szName) const override;

        // [LYN-2376] Remove once legacy slice support is removed
        auto GetLevelPackOpenEvent() -> LevelPackOpenEvent* override;
        auto GetLevelPackCloseEvent()->LevelPackCloseEvent* override;


        // Return cached file data for entries inside archive file.
        CCachedFileDataPtr GetOpenedFileDataInZip(AZ::IO::HandleType file);
        ZipDir::FileEntry* FindPakFileEntry(AZStd::string_view szPath, uint32_t& nArchiveFlags,
            ZipDir::CachePtr* pZip = {}, bool bSkipInMemoryArchives = {}) const;
    private:

        bool OpenPackCommon(AZStd::string_view szBindRoot, AZStd::string_view pName, uint32_t nArchiveFlags, AZStd::intrusive_ptr<AZ::IO::MemoryBlock> pData = nullptr, bool addLevels = true);
        bool OpenPacksCommon(AZStd::string_view szDir, AZStd::string_view pWildcardIn, uint32_t nArchiveFlags, AZStd::vector<AZ::IO::FixedMaxPathString>* pFullPaths = nullptr, bool addLevels = true);

        ZipDir::FileEntry* FindPakFileEntry(AZStd::string_view szPath) const;

        void CheckFileAccess(AZStd::string_view szFilename);

        // this function gets the file data for the given file, if found.
        // The file data object may be created in this function,
        // and it's important that the intrusive is returned: another thread may release the existing
        // cached data before the function returns
        // the path must be absolute normalized lower-case with forward-slashes
        CCachedFileDataPtr GetFileData(AZStd::string_view szName, uint32_t& nArchiveFlags, ZipDir::CachePtr* pZip = nullptr);
        // Get the data for a file by name within an archive if it exists
        CCachedFileDataPtr GetFileData(ZipDir::CachePtr pZip, AZStd::string_view szName);

        void LogFileAccessCallStack(AZStd::string_view name, AZStd::string_view nameFull, const char* mode);

        // Registers a non-owning pointer of the NestedArchive with the Archive instance
        void Register(INestedArchive* pArchive);
        void Unregister(INestedArchive* pArchive);
        INestedArchive* FindArchive(AZStd::string_view szFullPath) const;

        //! Return the Manifest from a bundle, if it exists
        AZStd::shared_ptr<AzFramework::AssetBundleManifest> GetBundleManifest(ZipDir::CachePtr pZip);
        AZStd::shared_ptr<AzFramework::AssetRegistry> GetBundleCatalog(ZipDir::CachePtr pZip, const AZStd::string& catalogName);

        // [LYN-2376] Remove once legacy slice support is removed
        AZStd::vector<AZStd::string> ScanForLevels(ZipDir::CachePtr pZip);

        mutable AZStd::shared_mutex m_csOpenFiles;
        ZipPseudoFileArray m_arrOpenFiles;
        CachedFileRawDataSet m_cachedFileRawDataSet;
        AZStd::mutex m_cachedFileRawDataMutex;
        // For m_pCachedFileRawDataSet
        using RawDataCacheLockGuard = AZStd::scoped_lock<decltype(m_cachedFileRawDataMutex)>;
        // The F* emulation functions critical section: protects all F* functions
        // that don't have a chance to be called recursively (to avoid deadlocks)
        AZStd::mutex m_csMain;
        mutable AZStd::shared_mutex m_archiveMutex;
        ArchiveArray m_arrArchives;

        mutable AZStd::shared_mutex m_csZips;
        ZipArray m_arrZips;

        //////////////////////////////////////////////////////////////////////////
        // Opened files collector.
        //////////////////////////////////////////////////////////////////////////

        IArchive::ERecordFileOpenList m_eRecordFileOpenList = RFOM_Disabled;
        using RecordedFilesSet = AZStd::set<AZ::OSString, AZ::IO::AZStdStringLessCaseInsensitive, AZ::OSStdAllocator>;
        RecordedFilesSet m_recordedFilesSet;

        AZStd::intrusive_ptr<IResourceList> m_pEngineStartupResourceList;

        // [LYN-2376] Remove once legacy slice support is removed
        AZStd::intrusive_ptr<IResourceList> m_pLevelResourceList;
        AZStd::intrusive_ptr<IResourceList> m_pNextLevelResourceList;

        float m_fFileAccessTime{}; // Time used to perform file operations
        AZStd::vector<IArchiveFileAccessSink*, AZ::OSStdAllocator> m_FileAccessSinks; // useful for gathering file access statistics

        bool m_disableRuntimeFileAccess[2]{};

        //threads which we don't want to access files from during the game
        AZStd::thread_id m_mainThreadId{};
        AZStd::thread_id m_renderThreadId{};

        AZStd::fixed_string<128> m_sLocalizationFolder;
        AZStd::fixed_string<128> m_sLocalizationRoot;

        AZStd::set<uint32_t, AZStd::less<>, AZ::OSStdAllocator> m_filesCachedOnHDD;

        // [LYN-2376] Remove once legacy slice support is removed
        LevelPackOpenEvent m_levelOpenEvent;
        LevelPackCloseEvent m_levelCloseEvent;
    };
}

namespace AZ::IO::ArchiveInternal
{
    // Utility function to de-alias archive file opening and file-within-archive opening
    // if the file specified was an absolute path but it points at one of the aliases, de-alias it and replace it with that alias.
    // this works around problems where the level editor is in control but still mounts asset packs (ie, level.pak mounted as @assets@)
    AZStd::optional<AZ::IO::FixedMaxPath> ConvertAbsolutePathToAliasedPath(AZStd::string_view sourcePath,
        AZStd::string_view aliasToLookFor = "@devassets@", AZStd::string_view aliasToReplaceWith = "@assets@");
}
