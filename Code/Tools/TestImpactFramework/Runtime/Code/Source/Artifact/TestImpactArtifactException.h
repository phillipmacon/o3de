/*
 * Copyright (c) Contributors to the Open 3D Engine Project
 * 
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <TestImpactFramework/TestImpactException.h>

namespace TestImpact
{
    //! Exception for artifacts and artifact parsing operations.
    class ArtifactException
        : public Exception
    {
    public:
        using Exception::Exception;
    };
} // namespace TestImpact
