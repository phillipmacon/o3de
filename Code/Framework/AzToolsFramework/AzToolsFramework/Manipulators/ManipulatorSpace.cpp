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

#include "ManipulatorSpace.h"

namespace AzToolsFramework
{
    const AZ::Transform& ManipulatorSpace::GetSpace() const
    {
        return m_space;
    }

    void ManipulatorSpace::SetSpace(const AZ::Transform& space)
    {
        m_space = space;
    }

    const AZ::Vector3& ManipulatorSpace::GetNonUniformScale() const
    {
        return m_nonUniformScale;
    }

    void ManipulatorSpace::SetNonUniformScale(const AZ::Vector3& nonUniformScale)
    {
        m_nonUniformScale = nonUniformScale;
    }

    AZ::Transform ManipulatorSpace::ApplySpace(const AZ::Transform& localTransform) const
    {
        AZ::Transform result;
        result.SetRotation(m_space.GetRotation() * localTransform.GetRotation());
        result.SetTranslation(m_space.TransformPoint(m_nonUniformScale * localTransform.GetTranslation()));
        result.SetScale(m_space.GetScale() * localTransform.GetUniformScale());
        return result;
    }

    const AZ::Vector3& ManipulatorSpaceWithLocalPosition::GetLocalPosition() const
    {
        return m_localPosition;
    }

    void ManipulatorSpaceWithLocalPosition::SetLocalPosition(const AZ::Vector3& localPosition)
    {
        m_localPosition = localPosition;
    }

    const AZ::Vector3& ManipulatorSpaceWithLocalTransform::GetLocalPosition() const
    {
        return m_localTransform.GetTranslation();
    }

    void ManipulatorSpaceWithLocalTransform::SetLocalPosition(const AZ::Vector3& localPosition)
    {
        m_localTransform.SetTranslation(localPosition);
    }

    const AZ::Transform& ManipulatorSpaceWithLocalTransform::GetLocalTransform() const
    {
        return m_localTransform;
    }

    const AZ::Quaternion& ManipulatorSpaceWithLocalTransform::GetLocalOrientation() const
    {
        return m_localTransform.GetRotation();
    }

    void ManipulatorSpaceWithLocalTransform::SetLocalTransform(const AZ::Transform& localTransform)
    {
        m_localTransform = localTransform;
    }

    void ManipulatorSpaceWithLocalTransform::SetLocalOrientation(const AZ::Quaternion& localOrientation)
    {
        m_localTransform.SetRotation(localOrientation);
    }
} // namespace AzToolsFramework
