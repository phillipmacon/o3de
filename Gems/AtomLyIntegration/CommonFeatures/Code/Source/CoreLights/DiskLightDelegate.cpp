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

#include <CoreLights/DiskLightDelegate.h>
#include <Atom/RPI.Public/Scene.h>
#include <AtomLyIntegration/CommonFeatures/CoreLights/AreaLightComponentConfig.h>

namespace AZ::Render
{
    DiskLightDelegate::DiskLightDelegate(LmbrCentral::DiskShapeComponentRequests* shapeBus, EntityId entityId, bool isVisible)
        : LightDelegateBase<DiskLightFeatureProcessorInterface>(entityId, isVisible)
        , m_shapeBus(shapeBus)
    {
        InitBase(entityId);
    }

    float DiskLightDelegate::CalculateAttenuationRadius(float lightThreshold) const
    {
        // Calculate the radius at which the irradiance will be equal to cutoffIntensity.
        float intensity = GetPhotometricValue().GetCombinedIntensity(PhotometricUnit::Lumen);
        return sqrt(intensity / lightThreshold);
    }

    void DiskLightDelegate::HandleShapeChanged()
    {
        if (GetLightHandle().IsValid())
        {
            GetFeatureProcessor()->SetPosition(GetLightHandle(), GetTransform().GetTranslation());
            GetFeatureProcessor()->SetDirection(GetLightHandle(), m_shapeBus->GetNormal());
            GetFeatureProcessor()->SetDiskRadius(GetLightHandle(), GetRadius());
        }
    }

    float DiskLightDelegate::GetSurfaceArea() const
    {
        float radius = GetRadius();
        return Constants::Pi * radius * radius;
    }

    float DiskLightDelegate::GetRadius() const
    {
        return m_shapeBus->GetRadius() * GetTransform().GetScale().GetMaxElement();
    }

    void DiskLightDelegate::DrawDebugDisplay(const Transform& transform, const Color& /*color*/, AzFramework::DebugDisplayRequests& debugDisplay, bool isSelected) const
    {
        if (isSelected)
        {
            debugDisplay.PushMatrix(transform);
            float radius = GetConfig()->m_attenuationRadius;

            if (GetConfig()->m_enableShutters)
            {

                float innerRadians = DegToRad(GetConfig()->m_innerShutterAngleDegrees);
                float outerRadians = DegToRad(GetConfig()->m_outerShutterAngleDegrees);

                // Draw a cone using the cone angle and attenuation radius
                innerRadians = GetMin(innerRadians, outerRadians);
                float coneRadiusInner = sin(innerRadians) * radius;
                float coneHeightInner = cos(innerRadians) * radius;
                float coneRadiusOuter = sin(outerRadians) * radius;
                float coneHeightOuter = cos(outerRadians) * radius;

                auto DrawConicalFrustum = [&debugDisplay](uint32_t numRadiusLines, float topRadius, float bottomRadius, float height, float brightness)
                {
                    debugDisplay.SetColor(Color(brightness, brightness, brightness, 1.0f));
                    debugDisplay.DrawWireDisk(Vector3(0.0, 0.0, height), Vector3::CreateAxisZ(), bottomRadius);
            
                    for (uint32_t i = 0; i < numRadiusLines; ++i)
                    {
                        float radiusLineAngle = float(i) / numRadiusLines * Constants::TwoPi;
                        debugDisplay.DrawLine(
                            Vector3(cos(radiusLineAngle) * topRadius, sin(radiusLineAngle) * topRadius, 0),
                            Vector3(cos(radiusLineAngle) * bottomRadius, sin(radiusLineAngle) * bottomRadius, height)
                        );
                    }
                };
            
                DrawConicalFrustum(16, m_shapeBus->GetRadius(), m_shapeBus->GetRadius() + coneRadiusInner, coneHeightInner, 1.0f);
                DrawConicalFrustum(16, m_shapeBus->GetRadius(), m_shapeBus->GetRadius() + coneRadiusOuter, coneHeightOuter, 0.65f);

            }
            else
            {
                debugDisplay.DrawWireDisk(Vector3::CreateZero(), Vector3::CreateAxisZ(), radius);
                debugDisplay.DrawArc(Vector3::CreateZero(), radius, 270.0f, 180.0f, 3.0f, 0);
                debugDisplay.DrawArc(Vector3::CreateZero(), radius, 0.0f, 180.0f, 3.0f, 1);
            }
            debugDisplay.PopMatrix();
        }
    }
    
    void DiskLightDelegate::SetEnableShutters(bool enabled)
    {
        Base::SetEnableShutters(enabled);
        GetFeatureProcessor()->SetConstrainToConeLight(GetLightHandle(), true);
    }

    void DiskLightDelegate::SetShutterAngles(float innerAngleDegrees, float outerAngleDegrees)
    {
        if (GetShuttersEnabled())
        {
            GetFeatureProcessor()->SetConeAngles(GetLightHandle(), DegToRad(innerAngleDegrees), DegToRad(outerAngleDegrees));
        }
    }

    void DiskLightDelegate::SetEnableShadow(bool enabled)
    {
        Base::SetEnableShadow(enabled);
        GetFeatureProcessor()->SetShadowsEnabled(GetLightHandle(), enabled);
    }

    void DiskLightDelegate::SetShadowmapMaxSize(ShadowmapSize size)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetShadowmapMaxResolution(GetLightHandle(), size);
        }
    }

    void DiskLightDelegate::SetShadowFilterMethod(ShadowFilterMethod method)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetShadowFilterMethod(GetLightHandle(), method);
        }
    }

    void DiskLightDelegate::SetSofteningBoundaryWidthAngle(float widthInDegrees)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetSofteningBoundaryWidthAngle(GetLightHandle(), DegToRad(widthInDegrees));
        }
    }

    void DiskLightDelegate::SetPredictionSampleCount(uint32_t count)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetPredictionSampleCount(GetLightHandle(), count);
        }
    }

    void DiskLightDelegate::SetFilteringSampleCount(uint32_t count)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetFilteringSampleCount(GetLightHandle(), count);
        }
    }

    void DiskLightDelegate::SetPcfMethod(PcfMethod method)
    {
        if (GetShadowsEnabled())
        {
            GetFeatureProcessor()->SetPcfMethod(GetLightHandle(), method);
        }
    }

    
} // namespace AZ::Render
