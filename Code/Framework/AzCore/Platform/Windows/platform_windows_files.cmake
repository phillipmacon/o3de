#
# Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
# 
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    AzCore/AzCore_Traits_Platform.h
    AzCore/AzCore_Traits_Windows.h
    AzCore/base_Platform.h
    AzCore/base_Windows.h
    AzCore/PlatformId/PlatformId_Platform.h
    AzCore/PlatformId/PlatformId_Windows.h
    ../Common/WinAPI/AzCore/std/parallel/internal/condition_variable_WinAPI.h
    ../Common/WinAPI/AzCore/std/parallel/internal/mutex_WinAPI.h
    ../Common/WinAPI/AzCore/std/parallel/internal/semaphore_WinAPI.h
    ../Common/WinAPI/AzCore/std/parallel/internal/thread_WinAPI.cpp
    ../Common/WinAPI/AzCore/std/parallel/internal/thread_WinAPI.h
    AzCore/std/parallel/internal/condition_variable_Platform.h
    AzCore/std/parallel/internal/mutex_Platform.h
    AzCore/std/parallel/internal/semaphore_Platform.h
    AzCore/std/parallel/internal/thread_Platform.h
    AzCore/std/parallel/internal/thread_Windows.cpp
    ../Common/WinAPI/AzCore/std/parallel/config_WinAPI.h
    AzCore/std/parallel/config_Platform.h
    AzCore/std/string/fixed_string_Platform.inl
    ../Common/MSVC/AzCore/std/string/fixed_string_MSVC.inl
    ../Common/VisualStudio/AzCore/Natvis/azcore.natvis
    ../Common/VisualStudio/AzCore/Natvis/azcore.natstepfilter
    ../Common/VisualStudio/AzCore/Natvis/azcore.natjmc
    AzCore/Debug/StackTracer_Windows.cpp
    ../Common/WinAPI/AzCore/Debug/Trace_WinAPI.cpp
    ../Common/WinAPI/AzCore/IO/Streamer/StreamerContext_WinAPI.cpp
    ../Common/WinAPI/AzCore/IO/Streamer/StreamerContext_WinAPI.h
    ../Common/WinAPI/AzCore/IO/SystemFile_WinAPI.cpp
    ../Common/WinAPI/AzCore/IO/SystemFile_WinAPI.h
    AzCore/IO/SystemFile_Platform.h
    AzCore/IO/Streamer/StorageDrive_Windows.h
    AzCore/IO/Streamer/StorageDrive_Windows.cpp
    AzCore/IO/Streamer/StorageDriveConfig_Windows.h
    AzCore/IO/Streamer/StorageDriveConfig_Windows.cpp
    AzCore/IO/Streamer/StreamerConfiguration_Windows.h
    AzCore/IO/Streamer/StreamerConfiguration_Windows.cpp
    AzCore/IO/Streamer/StreamerContext_Platform.h
    AzCore/IPC/SharedMemory_Platform.h
    AzCore/IPC/SharedMemory_Windows.h
    AzCore/IPC/SharedMemory_Windows.cpp
    ../Common/WinAPI/AzCore/Memory/OSAllocator_WinAPI.h
    ../Common/WinAPI/AzCore/Memory/OverrunDetectionAllocator_WinAPI.h
    AzCore/Memory/HeapSchema_Windows.cpp
    AzCore/Memory/OSAllocator_Platform.h
    AzCore/Memory/OverrunDetectionAllocator_Platform.h
    AzCore/Math/Random_Platform.h
    AzCore/Math/Random_Windows.cpp
    AzCore/Math/Random_Windows.h
    AzCore/Module/Internal/ModuleManagerSearchPathTool_Windows.cpp
    AzCore/Math/Internal/MathTypes_Windows.h
    ../Common/WinAPI/AzCore/Module/DynamicModuleHandle_WinAPI.cpp
    AzCore/NativeUI/NativeUISystemComponent_Windows.cpp
    AzCore/Platform_Windows.cpp
    AzCore/PlatformIncl_Platform.h
    AzCore/PlatformIncl_Windows.h
    ../Common/WinAPI/AzCore/Socket/AzSocket_fwd_WinAPI.h
    ../Common/WinAPI/AzCore/Socket/AzSocket_WinAPI.cpp
    ../Common/WinAPI/AzCore/Socket/AzSocket_WinAPI.h
    AzCore/Socket/AzSocket_Platform.h
    AzCore/Socket/AzSocket_fwd_Platform.h
    AzCore/Socket/AzSocket_fwd_Windows.h
    AzCore/std/time_Windows.cpp
    ../Common/WinAPI/AzCore/Utils/Utils_WinAPI.cpp
    AzCore/Utils/Utils_Windows.cpp
)
