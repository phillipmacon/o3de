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

#include "ComparisonFunctions.h"
#include <Libraries/Core/BinaryOperator.h>

namespace ScriptCanvas
{
    namespace Nodes
    {
        namespace Comparison
        {
            class Greater
                : public ComparisonExpression
            {
            public:
                AZ_COMPONENT(Greater, "{218F5872-8D89-4FEA-9761-662625E29580}", ComparisonExpression);

                static void Reflect(AZ::ReflectContext* reflection)
                {
                    if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
                    {
                        serializeContext->Class<Greater, ComparisonExpression>()
                            ->Version(0)
                            ;

                        if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                        {
                            editContext->Class<Greater>("Greater Than (>)", "Checks if Value A is greater than Value B")
                                ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/ScriptCanvas/Placeholder.png")
                                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                                ;
                        }
                    }
                }

            protected:
                Datum Evaluate(const Datum& lhs, const Datum& rhs) override
                {
                    ComparisonOutcome result(lhs >  rhs);
                    if (result.IsSuccess())
                    {
                        return Datum(result.GetValue());
                    }
                    else
                    {
                        SCRIPTCANVAS_REPORT_ERROR((*this), result.GetError().c_str());
                        return Datum(false);
                    }
                }
            };

        }
    }
}

