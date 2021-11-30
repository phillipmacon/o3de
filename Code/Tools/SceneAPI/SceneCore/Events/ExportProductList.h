/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/base.h>
#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Math/Uuid.h>
#include <SceneAPI/SceneCore/SceneCoreConfiguration.h>

namespace AZ
{
    class ReflectContext;

    namespace SceneAPI
    {
        namespace Events
        {
            struct ExportProduct
            {
                SCENE_CORE_API ExportProduct(const AZStd::string& filename, Uuid id, Data::AssetType assetType, AZStd::optional<u8> lod, AZStd::optional<u32> subId,
                    Data::ProductDependencyInfo::ProductDependencyFlags dependencyFlags = Data::ProductDependencyInfo::CreateFlags(Data::AssetLoadBehavior::NoLoad));
                SCENE_CORE_API ExportProduct(AZStd::string&& filename, Uuid id, Data::AssetType assetType, AZStd::optional<u8> lod, AZStd::optional<u32> subId,
                    Data::ProductDependencyInfo::ProductDependencyFlags dependencyFlags = Data::ProductDependencyInfo::CreateFlags(Data::AssetLoadBehavior::NoLoad));
                ExportProduct() = default;
                ExportProduct(const ExportProduct& rhs) = default;
                SCENE_CORE_API ExportProduct(ExportProduct&& rhs);

                ExportProduct& operator=(const ExportProduct& rhs) = default;
                SCENE_CORE_API ExportProduct& operator=(ExportProduct&& rhs);

                //! Other names the product file may be known as in the past. This is only backwards compatibility in ResourceCompilerScene.
                AZStd::vector<AZStd::string> m_legacyFileNames;
                //! Relative or absolute path of the product file.
                AZStd::string m_filename;
                //! Unique id for the product file. This is usually based on the group id and is used to generate the 
                //! the sub id.
                Uuid m_id;
                //! Type of the product file.
                Data::AssetType m_assetType;

                AZStd::optional<u32> m_subId;
                //! If the product makes use of level of detail, the level is encoded in the sub id. Otherwise the entire sub id number will be used for the product id.
                AZStd::optional<u8> m_lod;
                //! Save off any product dependency flags that are detected for any serialized dependencies.
                Data::ProductDependencyInfo::ProductDependencyFlags m_dependencyFlags;
                //! Relative path dependencies for autogenerated FBX materials
                AZStd::vector<AZStd::string> m_legacyPathDependencies;
                //! In the case of CGFs, we will have LOD export products that are dependencies of the base LOD
                AZStd::vector<ExportProduct> m_productDependencies;
            };

            class ExportProductList
            {
            public:
                static void Reflect(ReflectContext* context);

                SCENE_CORE_API ExportProduct& AddProduct(const AZStd::string& filename, Uuid id, Data::AssetType assetType, AZStd::optional<u8> lod, AZStd::optional<u32> subId,
                    Data::ProductDependencyInfo::ProductDependencyFlags dependencyFlags = Data::ProductDependencyInfo::CreateFlags(Data::AssetLoadBehavior::NoLoad));
                SCENE_CORE_API ExportProduct& AddProduct(AZStd::string&& filename, Uuid id, Data::AssetType assetType, AZStd::optional<u8> lod, AZStd::optional<u32> subId,
                    Data::ProductDependencyInfo::ProductDependencyFlags dependencyFlags = Data::ProductDependencyInfo::CreateFlags(Data::AssetLoadBehavior::NoLoad));

                SCENE_CORE_API const AZStd::vector<ExportProduct>& GetProducts() const;

                SCENE_CORE_API void AddDependencyToProduct(const AZStd::string& productName, ExportProduct& dependency);

            private:
                AZStd::vector<ExportProduct> m_products;
            };
        } // namespace Events
    } // namespace SceneAPI
} // namespace AZ

namespace AZ
{
    AZ_TYPE_INFO_SPECIALIZE(SceneAPI::Events::ExportProduct, "{6054EDCB-4C04-4D96-BF26-704999FFB725}");
    AZ_TYPE_INFO_SPECIALIZE(SceneAPI::Events::ExportProductList, "{1C76A51F-431B-4987-B653-CFCC940D0D0F}");
}
