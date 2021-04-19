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

#include <AzCore/Serialization/Json/ByteStreamSerializer.h>
#include <Tests/Serialization/Json/BaseJsonSerializerFixture.h>
#include <Tests/Serialization/Json/JsonSerializerConformityTests.h>

namespace JsonSerializationTests
{
    class ByteStreamSerializerTestDescription : public JsonSerializerConformityTestDescriptor<AZ::JsonByteStream>
    {
    public:
        AZStd::shared_ptr<AZ::BaseJsonSerializer> CreateSerializer() override
        {
            return AZStd::make_shared<AZ::JsonByteStreamSerializer>();
        }

        AZStd::shared_ptr<AZ::JsonByteStream> CreateDefaultInstance() override
        {
            return AZStd::make_shared<AZ::JsonByteStream>();
        }

        AZStd::shared_ptr<AZ::JsonByteStream> CreateFullySetInstance() override
        {
            // create a JsonByteStream (AZStd::vector<u8>) with ten 'a's
            return AZStd::make_shared<AZ::JsonByteStream>(10, 'a');
        }

        AZStd::string_view GetJsonForFullySetInstance() override
        {
            // Base64 encoded version of 'aaaaaaaaaa' (see CreateFullySetInstance)
            return R"("YWFhYWFhYWFhYQ==")";
        }

        void ConfigureFeatures(JsonSerializerConformityTestDescriptorFeatures& features) override
        {
            features.EnableJsonType(rapidjson::kStringType);
            features.m_supportsPartialInitialization = false;
            features.m_supportsInjection = false;
        }

        bool AreEqual(const AZ::JsonByteStream& lhs, const AZ::JsonByteStream& rhs) override
        {
            return lhs == rhs;
        }
    };

    using ByteStreamConformityTestTypes = ::testing::Types<ByteStreamSerializerTestDescription>;
    INSTANTIATE_TYPED_TEST_CASE_P(JsonByteStreamSerialzier, JsonSerializerConformityTests, ByteStreamConformityTestTypes);
} // namespace JsonSerializationTests
