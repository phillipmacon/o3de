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
#pragma once

#include <AzCore/Serialization/EditContext.h>

namespace AWSClientAuth
{
    //! Holds Login with Amazon provider serialized settings
    class LWAProviderSetting
    {
    public:
        LWAProviderSetting() = default;

        ~LWAProviderSetting() = default;

        AZ_TYPE_INFO(LWAProviderSetting, "{AA7A0B0E-626B-4D87-B862-8961BA0C843E}");

        AZStd::string m_appClientId;
        AZStd::string m_responseType;
        AZStd::string m_grantType;
        AZStd::string m_oAuthCodeURL;
        AZStd::string m_oAuthTokensURL;

        static void Reflect(AZ::SerializeContext& context)
        {
            context.Class<LWAProviderSetting>()
                ->Field("AppClientId", &LWAProviderSetting::m_appClientId)
                ->Field("ResponseType", &LWAProviderSetting::m_responseType)
                ->Field("GrantType", &LWAProviderSetting::m_grantType)
                ->Field("OAuthCodeURL", &LWAProviderSetting::m_oAuthCodeURL)
                ->Field("OAuthTokensURL", &LWAProviderSetting::m_oAuthTokensURL);
        }
    };

    //! Holds Google provider serialized settings
    class GoogleProviderSetting
    {
    public:
        GoogleProviderSetting() = default;

        ~GoogleProviderSetting() = default;

        AZ_TYPE_INFO(GoogleProviderSetting, "{C501E0DB-EEA5-4BC9-8E2E-CAE037AEE95F}");

        AZStd::string m_appClientId;
        AZStd::string m_clientSecret;
        AZStd::string m_grantType;
        AZStd::string m_oAuthCodeURL;
        AZStd::string m_oAuthTokensURL;

        static void Reflect(AZ::SerializeContext& context)
        {
            context.Class<GoogleProviderSetting>()
                ->Field("AppClientId", &GoogleProviderSetting::m_appClientId)
                ->Field("ClientSecret", &GoogleProviderSetting::m_clientSecret)
                ->Field("GrantType", &GoogleProviderSetting::m_grantType)
                ->Field("OAuthCodeURL", &GoogleProviderSetting::m_oAuthCodeURL)
                ->Field("OAuthTokensURL", &GoogleProviderSetting::m_oAuthTokensURL);
        }
    };
} // namespace AWSClientAuth
