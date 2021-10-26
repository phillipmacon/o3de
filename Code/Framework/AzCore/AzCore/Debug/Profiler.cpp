/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Debug/Profiler.h>
#include <AzCore/Debug/ProfilerBus.h>
#include <AzCore/Console/IConsole.h>
#include <AzCore/Console/ILogger.h>
#include <AzCore/Settings/SettingsRegistry.h>

namespace AZ::Debug
{
    AZStd::string GenerateOutputFile(const char* nameHint)
    {
        AZStd::string timeString;
        AZStd::to_string(timeString, AZStd::GetTimeNowSecond());

        AZ::IO::FixedMaxPathString captureOutput = GetProfilerCaptureLocation();

        return AZStd::string::format("%s/capture_%s_%s.json", captureOutput.c_str(), nameHint, timeString.c_str());
    }

    void ProfilerCaptureFrame([[maybe_unused]] const AZ::ConsoleCommandContainer& arguments)
    {
        AZStd::string captureFile = GenerateOutputFile("single");
        AZLOG_INFO("Setting capture file to %s", captureFile.c_str());
        AZ::Debug::ProfilerRequestBus::Broadcast(&AZ::Debug::ProfilerRequestBus::Events::CaptureFrame, captureFile);
    }
    AZ_CONSOLEFREEFUNC(ProfilerCaptureFrame, AZ::ConsoleFunctorFlags::DontReplicate, "Capture a single frame of profiling data");

    void ProfilerStartCapture([[maybe_unused]] const AZ::ConsoleCommandContainer& arguments)
    {
        AZStd::string captureFile = GenerateOutputFile("multi");
        AZLOG_INFO("Setting capture file to %s", captureFile.c_str());
        ProfilerRequestBus::Broadcast(&ProfilerRequestBus::Events::StartCapture, captureFile);
    }
    AZ_CONSOLEFREEFUNC(ProfilerStartCapture, AZ::ConsoleFunctorFlags::DontReplicate, "Start a multi-frame capture of profiling data");

    void ProfilerEndCapture([[maybe_unused]] const AZ::ConsoleCommandContainer& arguments)
    {
        AZ::Debug::ProfilerRequestBus::Broadcast(&AZ::Debug::ProfilerRequestBus::Events::EndCapture);
    }
    AZ_CONSOLEFREEFUNC(ProfilerEndCapture, AZ::ConsoleFunctorFlags::DontReplicate, "End and dump an in-progress continuous capture");

    AZ::IO::FixedMaxPathString GetProfilerCaptureLocation()
    {
        AZ::IO::FixedMaxPathString captureOutput;
        if (AZ::SettingsRegistryInterface* settingsRegistry = AZ::SettingsRegistry::Get(); settingsRegistry)
        {
            settingsRegistry->Get(captureOutput, RegistryKey_ProfilerCaptureLocation);
        }

        if (captureOutput.empty())
        {
            captureOutput = ProfilerCaptureLocationFallback;
        }

        return captureOutput;
    }
} // namespace AZ::Debug
