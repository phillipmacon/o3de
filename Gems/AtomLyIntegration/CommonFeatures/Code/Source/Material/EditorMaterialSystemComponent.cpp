/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AtomLyIntegration/CommonFeatures/Material/EditorMaterialSystemComponentNotificationBus.h>
#include <Atom/RHI/Factory.h>
#include <Atom/RPI.Reflect/Asset/AssetUtils.h>
#include <AtomLyIntegration/CommonFeatures/Material/MaterialComponentBus.h>
#include <AtomToolsFramework/Util/Util.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/StringFunc/StringFunc.h>
#include <AzCore/Utils/Utils.h>
#include <AzFramework/Application/Application.h>
#include <AzToolsFramework/API/EditorAssetSystemAPI.h>
#include <AzToolsFramework/API/ViewPaneOptions.h>
#include <Editor/LyViewPaneNames.h>
#include <Material/EditorMaterialComponentInspector.h>
#include <Material/EditorMaterialSystemComponent.h>
#include <Previewer/CommonPreviewContent.h>

// Disables warning messages triggered by the Qt library
// 4251: class needs to have dll-interface to be used by clients of class 
// 4800: forcing value to bool 'true' or 'false' (performance warning)
AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option")
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QProcessEnvironment>
AZ_POP_DISABLE_WARNING

void InitMaterialEditorResources()
{
    //Must register qt resources from other modules
    Q_INIT_RESOURCE(InspectorWidget);
}

namespace AZ
{
    namespace Render
    {
        //! Main system component for the Atom Common Feature Gem's editor/tools module.
        void EditorMaterialSystemComponent::Reflect(AZ::ReflectContext* context)
        {
            if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serialize->Class<EditorMaterialSystemComponent, AZ::Component>()
                    ->Version(0);

                if (AZ::EditContext* ec = serialize->GetEditContext())
                {
                    ec->Class<EditorMaterialSystemComponent>("EditorMaterialSystemComponent", "System component that manages launching and maintaining connections the material editor.")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ;
                }
            }
        }

        void EditorMaterialSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
        {
            provided.push_back(AZ_CRC("EditorMaterialSystem", 0x5c93bc4e));
        }

        void EditorMaterialSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
        {
            incompatible.push_back(AZ_CRC("EditorMaterialSystem", 0x5c93bc4e));
        }

        void EditorMaterialSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
        {
            AZ_UNUSED(dependent);
        }

        void EditorMaterialSystemComponent::Init()
        {
            InitMaterialEditorResources();
        }

        void EditorMaterialSystemComponent::Activate()
        {
            EditorMaterialSystemComponentRequestBus::Handler::BusConnect();
            AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusConnect();
            AzToolsFramework::EditorMenuNotificationBus::Handler::BusConnect();
            AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
            AzFramework::AssetCatalogEventBus::Handler::BusConnect();
            AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusConnect();
        }

        void EditorMaterialSystemComponent::Deactivate()
        {
            AzFramework::ApplicationLifecycleEvents::Bus::Handler::BusDisconnect();
            AzFramework::AssetCatalogEventBus::Handler::BusDisconnect();
            EditorMaterialSystemComponentRequestBus::Handler::BusDisconnect();
            AzToolsFramework::AssetBrowser::AssetBrowserInteractionNotificationBus::Handler::BusDisconnect();
            AzToolsFramework::EditorMenuNotificationBus::Handler::BusDisconnect();
            AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect(); 

            m_previewRenderer.reset();
            m_materialBrowserInteractions.reset();

            if (m_openMaterialEditorAction)
            {
                delete m_openMaterialEditorAction;
                m_openMaterialEditorAction = nullptr;
            }
        }

        void EditorMaterialSystemComponent::OpenMaterialEditor(const AZStd::string& sourcePath)
        {
            AZ_TracePrintf("MaterialComponent", "Launching Material Editor");

            QStringList arguments;
            arguments.append(sourcePath.c_str());

            // Use the same RHI as the main editor
            AZ::Name apiName = AZ::RHI::Factory::Get().GetName();
            if (!apiName.IsEmpty())
            {
                arguments.append(QString("--rhi=%1").arg(apiName.GetCStr()));
            }

            AZ::IO::FixedMaxPathString projectPath(AZ::Utils::GetProjectPath());
            if (!projectPath.empty())
            {
                arguments.append(QString("--project-path=%1").arg(projectPath.c_str()));
            }

            AtomToolsFramework::LaunchTool("MaterialEditor", AZ_TRAIT_OS_EXECUTABLE_EXTENSION, arguments);
        }

        void EditorMaterialSystemComponent::OpenMaterialInspector(
            const AZ::EntityId& entityId, const AZ::Render::MaterialAssignmentId& materialAssignmentId)
        {
            auto dockWidget = AzToolsFramework::InstanceViewPane("Material Property Inspector");
            if (dockWidget)
            {
                auto inspector = static_cast<AZ::Render::EditorMaterialComponentInspector::MaterialPropertyInspector*>(dockWidget->widget());
                if (inspector)
                {
                    inspector->LoadMaterial(entityId, materialAssignmentId);
                }
            }
        }

        void EditorMaterialSystemComponent::RenderMaterialPreview(
            const AZ::EntityId& entityId, const AZ::Render::MaterialAssignmentId& materialAssignmentId)
        {
            static constexpr const char* DefaultModelPath = "models/sphere.azmodel";
            static constexpr const char* DefaultLightingPresetPath = "lightingpresets/thumbnail.lightingpreset.azasset";

            if (m_previewRenderer)
            {
                AZ::Data::AssetId materialAssetId = {};
                MaterialComponentRequestBus::EventResult(
                    materialAssetId, entityId, &MaterialComponentRequestBus::Events::GetMaterialOverride, materialAssignmentId);
                if (!materialAssetId.IsValid())
                {
                    MaterialComponentRequestBus::EventResult(
                        materialAssetId, entityId, &MaterialComponentRequestBus::Events::GetDefaultMaterialAssetId, materialAssignmentId);
                }

                AZ::Render::MaterialPropertyOverrideMap propertyOverrides;
                AZ::Render::MaterialComponentRequestBus::EventResult(
                    propertyOverrides, entityId, &AZ::Render::MaterialComponentRequestBus::Events::GetPropertyOverrides,
                    materialAssignmentId);

                m_previewRenderer->AddCaptureRequest(
                    { 128,
                      AZStd::make_shared<AZ::LyIntegration::CommonPreviewContent>(
                          m_previewRenderer->GetScene(), m_previewRenderer->GetView(), m_previewRenderer->GetEntityContextId(),
                          AZ::RPI::AssetUtils::GetAssetIdForProductPath(DefaultModelPath), materialAssetId,
                          AZ::RPI::AssetUtils::GetAssetIdForProductPath(DefaultLightingPresetPath), propertyOverrides),
                      []()
                      {
                          // failed
                      },
                      [entityId, materialAssignmentId](const QPixmap& pixmap)
                      {
                          AZ::Render::EditorMaterialSystemComponentNotificationBus::Broadcast(
                              &AZ::Render::EditorMaterialSystemComponentNotificationBus::Events::OnRenderMaterialPreviewComplete, entityId,
                              materialAssignmentId, pixmap);
                      } });
            }
        }

        void EditorMaterialSystemComponent::OnPopulateToolMenuItems()
        {
            if (!m_openMaterialEditorAction)
            {
                m_openMaterialEditorAction = new QAction("Material Editor");
                m_openMaterialEditorAction->setShortcut(QKeySequence(Qt::Key_M));
                m_openMaterialEditorAction->setCheckable(false);
                m_openMaterialEditorAction->setChecked(false);
                m_openMaterialEditorAction->setIcon(QIcon(":/Menu/material_editor.svg"));
                QObject::connect(
                    m_openMaterialEditorAction, &QAction::triggered, m_openMaterialEditorAction, [this]()
                    {
                        OpenMaterialEditor("");
                    }
                );

                AzToolsFramework::EditorMenuRequestBus::Broadcast(
                    &AzToolsFramework::EditorMenuRequestBus::Handler::AddMenuAction, "ToolMenu", m_openMaterialEditorAction, true);
            }
        }

        void EditorMaterialSystemComponent::OnResetToolMenuItems()
        {
            if (m_openMaterialEditorAction)
            {
                delete m_openMaterialEditorAction;
                m_openMaterialEditorAction = nullptr;
            }
        }

        void EditorMaterialSystemComponent::NotifyRegisterViews()
        {
            AzToolsFramework::ViewPaneOptions inspectorOptions;
            inspectorOptions.canHaveMultipleInstances = true;
            inspectorOptions.preferedDockingArea = Qt::NoDockWidgetArea;
            inspectorOptions.paneRect = QRect(50, 50, 400, 700);
            inspectorOptions.showInMenu = false;
            inspectorOptions.showOnToolsToolbar = false;
            AzToolsFramework::RegisterViewPane<AZ::Render::EditorMaterialComponentInspector::MaterialPropertyInspector>(
                "Material Property Inspector", LyViewPane::CategoryTools, inspectorOptions);
        }

        void EditorMaterialSystemComponent::OnCatalogLoaded([[maybe_unused]] const char* catalogFile)
        {
            AZ::TickBus::QueueFunction([this](){
                m_materialBrowserInteractions.reset(aznew MaterialBrowserInteractions);
                m_previewRenderer.reset(aznew AtomToolsFramework::PreviewRenderer(
                    "EditorMaterialSystemComponent Preview Scene", "EditorMaterialSystemComponent Preview Pipeline"));
            });
        }

        void EditorMaterialSystemComponent::OnApplicationAboutToStop()
        {
            m_previewRenderer.reset();
            m_materialBrowserInteractions.reset();
        }

        AzToolsFramework::AssetBrowser::SourceFileDetails EditorMaterialSystemComponent::GetSourceFileDetails(
            const char* fullSourceFileName)
        {
            static const char* MaterialTypeIconPath = ":/Icons/materialtype.svg";
            static const char* MaterialTypeExtension = "materialtype";
            if (AzFramework::StringFunc::EndsWith(fullSourceFileName, MaterialTypeExtension))
            {
                return AzToolsFramework::AssetBrowser::SourceFileDetails(MaterialTypeIconPath);
            }
            return AzToolsFramework::AssetBrowser::SourceFileDetails();
        }
    } // namespace Render
} // namespace AZ
