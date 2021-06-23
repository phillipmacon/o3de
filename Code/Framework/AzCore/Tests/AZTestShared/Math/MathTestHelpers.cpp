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

#include "MathTestHelpers.h"

#include <AzCore/Math/Matrix3x3.h>
#include <AzCore/Math/Matrix4x4.h>
#include <AzCore/Math/Quaternion.h>
#include <AzCore/Math/Sphere.h>
#include <AzCore/Math/ToString.h>
#include <AzCore/Math/Vector2.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Math/Vector4.h>
#include <AzCore/Math/Aabb.h>
#include <AzCore/Math/Transform.h>
#include <AzCore/Math/Color.h>

// make gtest/gmock aware of these types so when a failure occurs we get more useful output
namespace AZ
{
    std::ostream& operator<<(std::ostream& os, const Vector2& vec)
    {
        return os
            << "(X: " << vec.GetX()
            << ", Y: " << vec.GetY()
            << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Vector3& vec)
    {
        return os
            << "(X: " << vec.GetX()
            << ", Y: " << vec.GetY()
            << ", Z: " << vec.GetZ()
            << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Vector4& vec)
    {
        return os
            << "(X: " << vec.GetX()
            << ", Y: " << vec.GetY()
            << ", Z: " << vec.GetZ()
            << ", W: " << vec.GetW()
            << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Aabb& aabb)
    {
        return os << "(min: " << aabb.GetMin() << ", max: " << aabb.GetMax() << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
    {
        return os
            << "(X: " << quat.GetX()
            << ", Y: " << quat.GetY()
            << ", Z: " << quat.GetZ()
            << ", W: " << quat.GetW()
            << ")";
    }

    std::ostream& operator<<(std::ostream& os, const Transform& transform)
    {
        return os
            << "translation: " << transform.GetTranslation()
            << " rotation: " << transform.GetRotation()
            << " scale: " << transform.GetUniformScale();
    }

    std::ostream& operator<<(std::ostream& os, const Color& color)
    {
        return os
            << "red: " << color.GetR()
            << " green: " << color.GetG()
            << " blue: " << color.GetB()
            << " alpha: " << color.GetA();
    }

    std::ostream& operator<< (std::ostream& os, const Sphere& sphere)
    {
        os << "center: " << sphere.GetCenter() << " radius: " << sphere.GetRadius();
        return os;
    }

    // pretty-printer for matrices in test output
    template<typename Mat>
    std::ostream& PrintMatrix(std::ostream& os, const Mat& mat)
    {
        auto printElement = [&os, &mat](int64_t row, int64_t col) -> std::ostream&
        {
            const std::streamsize width = 10;
            os << std::setw(width) << std::fixed << mat.GetElement(row, col);
            return os;
        };

        os << "\n";
        for (int64_t row = 0; row < Mat::RowCount - 1; ++row)
        {
            for (int64_t col = 0; col < Mat::ColCount; ++col)
            {
                printElement(row, col) << ", ";
            }

            os << "\n";
        }

        for (int64_t col = 0; col < Mat::ColCount - 1; ++col)
        {
            printElement(Mat::RowCount - 1, col) << ", ";
        }

        printElement(Mat::RowCount - 1, Mat::ColCount - 1);

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix3x3& mat)
    {
        return PrintMatrix(os, mat);
    }

    std::ostream& operator<<(std::ostream& os, const Matrix3x4& mat)
    {
        return PrintMatrix(os, mat);
    }

    std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat)
    {
        return PrintMatrix(os, mat);
    }
} // namespace AZ
