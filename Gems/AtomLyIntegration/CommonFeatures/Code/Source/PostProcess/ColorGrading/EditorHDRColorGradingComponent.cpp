/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <PostProcess/ColorGrading/EditorHDRColorGradingComponent.h>
#include <AzToolsFramework/API/EditorPythonRunnerRequestsBus.h>

namespace AZ
{
    namespace Render
    {
        void EditorHDRColorGradingComponent::Reflect(AZ::ReflectContext* context)
        {
            BaseClass::Reflect(context);

            if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<EditorHDRColorGradingComponent, BaseClass>()->Version(1);

                if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                {
                    editContext->Class<EditorHDRColorGradingComponent>(
                        "HDR Color Grading", "Tune and apply color grading in HDR.")
                        ->ClassElement(Edit::ClassElements::EditorData, "")
                        ->Attribute(Edit::Attributes::Category, "Atom")
                        ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg") // [GFX TODO ATOM-2672][PostFX] need to create icons for PostProcessing.
                        ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/Component_Placeholder.svg") // [GFX TODO ATOM-2672][PostFX] need to create icons for PostProcessing.
                        ->Attribute(Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                        ->Attribute(Edit::Attributes::AutoExpand, true)
                        ->Attribute(Edit::Attributes::HelpPageURL, "https://") // [TODO ATOM-2672][PostFX] need to create page for PostProcessing.
                        ->ClassElement(AZ::Edit::ClassElements::Group, "LUT Generation")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->UIElement(AZ::Edit::UIHandlers::Button, "Generate LUT", "Generates a LUT from the scene's enabled color grading blend.")
                            ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "")
                            ->Attribute(AZ::Edit::Attributes::ButtonText, "Generate LUT")
                            ->Attribute(AZ::Edit::Attributes::ChangeNotify, &EditorHDRColorGradingComponent::GenerateLut)
                        ;

                    editContext->Class<HDRColorGradingComponentController>(
                        "HDRColorGradingComponentControl", "")
                        ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Default, &HDRColorGradingComponentController::m_configuration, "Configuration", "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                        ;

                    editContext->Class<HDRColorGradingComponentConfig>("HDRColorGradingComponentConfig", "")
                        ->DataElement(Edit::UIHandlers::CheckBox, &HDRColorGradingComponentConfig::m_enabled,
                            "Enable HDR color grading",
                            "Enable HDR color grading.")
                        ->ClassElement(AZ::Edit::ClassElements::Group, "Color Adjustment")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingExposure, "Exposure", "Exposure Value")
                            ->Attribute(Edit::Attributes::Min, AZStd::numeric_limits<float>::lowest())
                            ->Attribute(Edit::Attributes::Max, AZStd::numeric_limits<float>::max())
                            ->Attribute(Edit::Attributes::SoftMin, -20.0f)
                            ->Attribute(Edit::Attributes::SoftMax, 20.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingContrast, "Contrast", "Contrast Value")
                            ->Attribute(Edit::Attributes::Min, -100.0f)
                            ->Attribute(Edit::Attributes::Max, 100.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingPreSaturation, "Pre Saturation", "Pre Saturation Value")
                            ->Attribute(Edit::Attributes::Min, -100.0f)
                            ->Attribute(Edit::Attributes::Max, 100.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingFilterIntensity, "Filter Intensity", "Filter Intensity Value")
                            ->Attribute(Edit::Attributes::Min, AZStd::numeric_limits<float>::lowest())
                            ->Attribute(Edit::Attributes::Max, AZStd::numeric_limits<float>::max())
                            ->Attribute(Edit::Attributes::SoftMin, -1.0f)
                            ->Attribute(Edit::Attributes::SoftMax, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingFilterMultiply, "Filter Multiply", "Filter Multiply Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_colorFilterSwatch, "Color Filter Swatch", "Color Filter Swatch Value")

                        ->ClassElement(AZ::Edit::ClassElements::Group, "White Balance")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_whiteBalanceKelvin, "Temperature", "Temperature in Kelvin")
                            ->Attribute(Edit::Attributes::Min, 1000.0f)
                            ->Attribute(Edit::Attributes::Max, 40000.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_whiteBalanceTint, "Tint", "Tint Value")
                            ->Attribute(Edit::Attributes::Min, -100.0f)
                            ->Attribute(Edit::Attributes::Max, 100.0f)

                        ->ClassElement(AZ::Edit::ClassElements::Group, "Split Toning")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_splitToneWeight, "Split Tone Weight", "Modulates the split toning effect.")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_splitToneBalance, "Split Tone Balance", "Split Tone Balance Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_splitToneShadowsColor, "Split Tone Shadows Color", "Split Tone Shadows Color")
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_splitToneHighlightsColor, "Split Tone Highlights Color", "Split Tone Highlights Color")

                        ->ClassElement(AZ::Edit::ClassElements::Group, "Channel Mixing")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_channelMixingRed, "Channel Mixing Red", "Channel Mixing Red Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_channelMixingGreen, "Channel Mixing Green", "Channel Mixing Green Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_channelMixingBlue, "Channel Mixing Blue", "Channel Mixing Blue Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)

                        ->ClassElement(AZ::Edit::ClassElements::Group, "Shadow Midtones Highlights")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_smhWeight, "SMH Weight", "Modulates the SMH effect.")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_smhShadowsStart, "SMH Shadows Start", "SMH Shadows Start Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_smhShadowsEnd, "SMH Shadows End", "SMH Shadows End Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_smhHighlightsStart, "SMH Highlights Start", "SMH Highlights Start Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_smhHighlightsEnd, "SMH Highlights End", "SMH Highlights End Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_smhShadowsColor, "SMH Shadows Color", "SMH Shadows Color")
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_smhMidtonesColor, "SMH Midtones Color", "SMH Midtones Color")
                        ->DataElement(AZ::Edit::UIHandlers::Color, &HDRColorGradingComponentConfig::m_smhHighlightsColor, "SMH Highlights Color", "SMH Highlights Color")

                        ->ClassElement(AZ::Edit::ClassElements::Group, "Final Adjustment")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingHueShift, "Hue Shift", "Hue Shift Value")
                            ->Attribute(Edit::Attributes::Min, 0.0f)
                            ->Attribute(Edit::Attributes::Max, 1.0f)
                        ->DataElement(AZ::Edit::UIHandlers::Slider, &HDRColorGradingComponentConfig::m_colorGradingPostSaturation, "Post Saturation", "Post Saturation Value")
                            ->Attribute(Edit::Attributes::Min, -100.0f)
                            ->Attribute(Edit::Attributes::Max, 100.0f)
                        ;
                }
            }
        }

        EditorHDRColorGradingComponent::EditorHDRColorGradingComponent(const HDRColorGradingComponentConfig& config)
            : BaseClass(config)
        {
        }

        void EditorHDRColorGradingComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
        {
            if (m_controller.m_configuration.m_generateLut && m_frameCounter)
            {
                --m_frameCounter;
                return;
            }
            else if (m_controller.m_configuration.m_generateLut && m_lutGenerationInProgress)
            {
                const char* LutAttachment = "LutOutput";
                const AZStd::vector<AZStd::string> LutGenerationPassHierarchy{ "LutGenerationPass" };

                // capture frame
                AZ::Render::FrameCaptureNotificationBus::Handler::BusConnect();

                bool startedCapture = false;
                AZ::Render::FrameCaptureRequestBus::BroadcastResult(
                    startedCapture,
                    &AZ::Render::FrameCaptureRequestBus::Events::CapturePassAttachment,
                    LutGenerationPassHierarchy,
                    AZStd::string(LutAttachment), TempTiffFilePath,
                    AZ::RPI::PassAttachmentReadbackOption::Output);

                m_lutGenerationInProgress = !startedCapture;
            }
        }

        void EditorHDRColorGradingComponent::OnCaptureFinished([[maybe_unused]] AZ::Render::FrameCaptureResult result, [[maybe_unused]]const AZStd::string& info)
        {
            char resolvedInputFilePath[AZ_MAX_PATH_LEN] = { 0 };
            AZ::IO::FileIOBase::GetDirectInstance()->ResolvePath(TempTiffFilePath, resolvedInputFilePath, AZ_MAX_PATH_LEN);
            char resolvedOutputFilePath[AZ_MAX_PATH_LEN] = { 0 };
            AZ::IO::FileIOBase::GetDirectInstance()->ResolvePath(GeneratedLutFilePath, resolvedOutputFilePath, AZ_MAX_PATH_LEN);

            AZStd::vector<AZStd::string_view> pythonArgs
            {
                "--i", resolvedInputFilePath,
                "--o", resolvedOutputFilePath
            };

            AzToolsFramework::EditorPythonRunnerRequestBus::Broadcast(
                &AzToolsFramework::EditorPythonRunnerRequestBus::Events::ExecuteByFilenameWithArgs,
                "@devroot@/Gems/Atom/Feature/Common/Editor/Scripts/ColorGrading/tiff_to_3dl_azasset.py", pythonArgs);

            m_controller.m_configuration.m_generateLut = false;
            m_controller.OnConfigChanged();

            AZ::TickBus::Handler::BusDisconnect();
            AZ::Render::FrameCaptureNotificationBus::Handler::BusDisconnect();
        }

        void EditorHDRColorGradingComponent::GenerateLut()
        {
            // turn on lut generation pass
            m_lutGenerationInProgress = true;
            m_controller.m_configuration.m_generateLut = true;
            m_controller.OnConfigChanged();

            m_frameCounter = FramesToWait;

            AZ::TickBus::Handler::BusConnect();
        }

        u32 EditorHDRColorGradingComponent::OnConfigurationChanged()
        {
            m_controller.OnConfigChanged();
            return Edit::PropertyRefreshLevels::AttributesAndValues;
        }
    } // namespace Render
} // namespace AZ
