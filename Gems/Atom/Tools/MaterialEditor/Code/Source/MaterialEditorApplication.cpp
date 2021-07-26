/*
 * Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.
 * 
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AtomToolsFramework/Util/Util.h>

#include <Atom/RPI.Edit/Common/AssetUtils.h>
#include <Atom/RPI.Public/RPISystemInterface.h>

#include <Atom/Document/MaterialDocumentModule.h>
#include <Atom/Document/MaterialDocumentSystemRequestBus.h>

#include <Atom/Viewport/MaterialViewportModule.h>

#include <Atom/Window/MaterialEditorWindowModule.h>
#include <Atom/Window/MaterialEditorWindowFactoryRequestBus.h>
#include <Atom/Window/MaterialEditorWindowRequestBus.h>

#include <AzCore/IO/Path/Path.h>
#include <AzCore/Utils/Utils.h>
#include <AzCore/Settings/SettingsRegistryMergeUtils.h>
#include <AzFramework/StringFunc/StringFunc.h>
#include <AzFramework/IO/LocalFileIO.h>
#include <AzFramework/Network/AssetProcessorConnection.h>
#include <AzFramework/Asset/AssetSystemComponent.h>

#include <AzToolsFramework/AzToolsFrameworkModule.h>
#include <AzToolsFramework/API/EditorPythonConsoleBus.h>
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>
#include <AzToolsFramework/UI/UICore/QWidgetSavedState.h>
#include <AzToolsFramework/UI/UICore/QTreeViewStateSaver.hxx>
#include <AzToolsFramework/AssetBrowser/AssetBrowserEntry.h>
#include <AzToolsFramework/SourceControl/PerforceComponent.h>
#include <AzToolsFramework/Asset/AssetSystemComponent.h>
#include <AzToolsFramework/AssetBrowser/AssetBrowserComponent.h>
#include <AzToolsFramework/Thumbnails/ThumbnailerComponent.h>
#include <AzToolsFramework/UI/PropertyEditor/PropertyManagerComponent.h>
#include <AzToolsFramework/SourceControl/SourceControlAPI.h>

#include <Source/MaterialEditorApplication.h>
#include <MaterialEditor_Traits_Platform.h>

AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option") // disable warnings spawned by QT
#include <QObject>
#include <QMessageBox>
AZ_POP_DISABLE_WARNING

namespace MaterialEditor
{
    //! This function returns the build system target name of "MaterialEditor
    AZStd::string MaterialEditorApplication::GetBuildTargetName()
    {
#if !defined (LY_CMAKE_TARGET)
#error "LY_CMAKE_TARGET must be defined in order to add this source file to a CMake executable target"
#endif
        return AZStd::string{ LY_CMAKE_TARGET };
    }

    const char* MaterialEditorApplication::GetCurrentConfigurationName() const
    {
#if defined(_RELEASE)
        return "ReleaseMaterialEditor";
#elif defined(_DEBUG)
        return "DebugMaterialEditor";
#else
        return "ProfileMaterialEditor";
#endif
    }

    MaterialEditorApplication::MaterialEditorApplication(int* argc, char*** argv)
        : AtomToolsApplication(argc, argv)

    {
        QApplication::setApplicationName("O3DE Material Editor");

        AZ::SettingsRegistryMergeUtils::MergeSettingsToRegistry_AddBuildSystemTargetSpecialization(
            *AZ::SettingsRegistry::Get(), GetBuildTargetName());

        connect(&m_timer, &QTimer::timeout, this, [&]()
        {
            this->PumpSystemEventLoopUntilEmpty();
            this->Tick();
        });
    }

    MaterialEditorApplication::~MaterialEditorApplication()
    {
        AzToolsFramework::AssetDatabase::AssetDatabaseRequestsBus::Handler::BusDisconnect();
        MaterialEditorWindowNotificationBus::Handler::BusDisconnect();
        AzToolsFramework::EditorPythonConsoleNotificationBus::Handler::BusDisconnect();
    }

    void MaterialEditorApplication::CreateStaticModules(AZStd::vector<AZ::Module*>& outModules)
    {
        Base::CreateStaticModules(outModules);
        outModules.push_back(aznew MaterialDocumentModule);
        outModules.push_back(aznew MaterialViewportModule);
        outModules.push_back(aznew MaterialEditorWindowModule);
    }

    void MaterialEditorApplication::OnMaterialEditorWindowClosing()
    {
        ExitMainLoop();
    }

    void MaterialEditorApplication::Destroy()
    {
        // before modules are unloaded, destroy UI to free up any assets it cached
        MaterialEditor::MaterialEditorWindowFactoryRequestBus::Broadcast(
            &MaterialEditor::MaterialEditorWindowFactoryRequestBus::Handler::DestroyMaterialEditorWindow);

        MaterialEditorWindowNotificationBus::Handler::BusDisconnect();

        Base::Destroy();
    }

    void MaterialEditorApplication::AssetSystemAvailable()
    {
        bool connectedToAssetProcessor = false;

        // When the AssetProcessor is already launched it should take less than a second to perform a connection
        // but when the AssetProcessor needs to be launch it could take up to 15 seconds to have the AssetProcessor initialize
        // and able to negotiate a connection when running a debug build
        // and to negotiate a connection

        AzFramework::AssetSystem::ConnectionSettings connectionSettings;
        AzFramework::AssetSystem::ReadConnectionSettingsFromSettingsRegistry(connectionSettings);
        connectionSettings.m_connectionDirection = AzFramework::AssetSystem::ConnectionSettings::ConnectionDirection::ConnectToAssetProcessor;
        connectionSettings.m_connectionIdentifier = GetBuildTargetName();
        connectionSettings.m_loggingCallback = []([[maybe_unused]] AZStd::string_view logData)
        {
            AZ_TracePrintf("Material Editor", "%.*s", aznumeric_cast<int>(logData.size()), logData.data());
        };
        AzFramework::AssetSystemRequestBus::BroadcastResult(connectedToAssetProcessor,
            &AzFramework::AssetSystemRequestBus::Events::EstablishAssetProcessorConnection, connectionSettings);

        if (connectedToAssetProcessor)
        {
            // List of common asset filters for things that need to be compiled to run the material editor
            // Some of these things will not be necessary once we have proper support for queued asset loading and reloading
            const AZStd::vector<AZStd::string> assetFiltersArray = { "passes/", "config/", "MaterialEditor/" };

            CompileCriticalAssets(assetFiltersArray);
        }

        AzFramework::AssetSystemStatusBus::Handler::BusDisconnect();
    }

    void MaterialEditorApplication::ProcessCommandLine(const AZ::CommandLine& commandLine)
    {
        const AZStd::string activateWindowSwitchName = "activatewindow";
        if (commandLine.HasSwitch(activateWindowSwitchName))
        {
            MaterialEditor::MaterialEditorWindowRequestBus::Broadcast(
                &MaterialEditor::MaterialEditorWindowRequestBus::Handler::ActivateWindow);
        }

        // Process command line options for opening one or more material documents on startup
        size_t openDocumentCount = commandLine.GetNumMiscValues();
        for (size_t openDocumentIndex = 0; openDocumentIndex < openDocumentCount; ++openDocumentIndex)
        {
            const AZStd::string openDocumentPath = commandLine.GetMiscValue(openDocumentIndex);

            AZ_Printf("MaterialEditor", "Opening document: %s", openDocumentPath.c_str());
            MaterialDocumentSystemRequestBus::Broadcast(&MaterialDocumentSystemRequestBus::Events::OpenDocument, openDocumentPath);
        }

        Base::ProcessCommandLine(commandLine);
    }

    void MaterialEditorApplication::StartInternal()
    {
        Base::StartInternal();

        MaterialEditorWindowNotificationBus::Handler::BusConnect();

        MaterialEditor::MaterialEditorWindowFactoryRequestBus::Broadcast(
            &MaterialEditor::MaterialEditorWindowFactoryRequestBus::Handler::CreateMaterialEditorWindow);
    }

    void MaterialEditorApplication::Stop()
    {
        MaterialEditor::MaterialEditorWindowFactoryRequestBus::Broadcast(
            &MaterialEditor::MaterialEditorWindowFactoryRequestBus::Handler::DestroyMaterialEditorWindow);

        Base::Stop();
    }
} // namespace MaterialEditor
