/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/DOM/Backends/JSON/JsonBackend.h>
#include <AzCore/DOM/Backends/JSON/JsonSerializationUtils.h>
#include <AzCore/DOM/DomUtils.h>
#include <AzCore/Name/NameDictionary.h>
#include <AzCore/Serialization/Json/JsonSerialization.h>
#include <AzCore/Serialization/Json/JsonUtils.h>
#include <AzCore/UnitTest/TestTypes.h>

namespace AZ::Dom::Tests
{
    class DomJsonTests : public UnitTest::AllocatorsFixture
    {
    public:
        void SetUp() override
        {
            UnitTest::AllocatorsFixture::SetUp();
            NameDictionary::Create();
            m_document = AZStd::make_unique<rapidjson::Document>();
        }

        void TearDown() override
        {
            m_document.reset();
            NameDictionary::Destroy();
            UnitTest::AllocatorsFixture::TearDown();
        }

        rapidjson::Value CreateString(const AZStd::string& text)
        {
            rapidjson::Value key;
            key.SetString(text.c_str(), static_cast<rapidjson::SizeType>(text.length()), m_document->GetAllocator());
            return key;
        }

        template<class T>
        void AddValue(const AZStd::string& key, T value)
        {
            m_document->AddMember(CreateString(key), rapidjson::Value(value), m_document->GetAllocator());
        }

        // Validate round-trip serialization to and from rapidjson::Document and a UTF-8 encoded string
        void PerformSerializationChecks()
        {
            // Generate a canonical serializaed representation of this document using rapidjson
            // This will be pretty-printed using the same rapidjson pretty printer we use, so should be binary identical
            // to any output generated by the visitor API
            AZStd::string canonicalSerializedDocument;
            AZ::JsonSerializationUtils::WriteJsonString(*m_document, canonicalSerializedDocument);

            auto visitDocumentFn = [this](AZ::Dom::Visitor& visitor)
            {
                return Json::VisitRapidJsonValue(*m_document, visitor, Lifetime::Persistent);
            };

            // Document -> Document
            {
                auto result = Json::WriteToRapidJsonDocument(visitDocumentFn);
                EXPECT_TRUE(result.IsSuccess());
                EXPECT_EQ(AZ::JsonSerialization::Compare(*m_document, result.GetValue()), AZ::JsonSerializerCompareResult::Equal);
            }

            // Document -> string
            {
                AZStd::string serializedDocument;
                JsonBackend backend;
                auto result = Dom::Utils::WriteToString(backend, serializedDocument, visitDocumentFn);
                EXPECT_TRUE(result.IsSuccess());
                EXPECT_EQ(canonicalSerializedDocument, serializedDocument);
            }

            // string -> Document
            {
                auto result = Json::WriteToRapidJsonDocument(
                    [&canonicalSerializedDocument](AZ::Dom::Visitor& visitor)
                    {
                        JsonBackend backend;
                        return Dom::Utils::ReadFromString(backend, canonicalSerializedDocument, Dom::Lifetime::Persistent, visitor);
                    });
                EXPECT_TRUE(result.IsSuccess());
                EXPECT_EQ(AZ::JsonSerialization::Compare(*m_document, result.GetValue()), JsonSerializerCompareResult::Equal);
            }

            // string -> string
            {
                AZStd::string serializedDocument;
                JsonBackend backend;
                auto result = Dom::Utils::WriteToString(
                    backend, serializedDocument,
                    [&backend, &canonicalSerializedDocument](AZ::Dom::Visitor& visitor)
                    {
                        return Dom::Utils::ReadFromString(backend, canonicalSerializedDocument, Dom::Lifetime::Persistent, visitor);
                    });
                EXPECT_TRUE(result.IsSuccess());
                EXPECT_EQ(canonicalSerializedDocument, serializedDocument);
            }
        }

        AZStd::unique_ptr<rapidjson::Document> m_document;
    };

    TEST_F(DomJsonTests, EmptyArray)
    {
        m_document->SetArray();
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, SimpleArray)
    {
        m_document->SetArray();
        for (int i = 0; i < 5; ++i)
        {
            m_document->PushBack(i, m_document->GetAllocator());
        }
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, NestedArrays)
    {
        m_document->SetArray();
        for (int j = 0; j < 7; ++j)
        {
            rapidjson::Value nestedArray(rapidjson::kArrayType);
            for (int i = 0; i < 5; ++i)
            {
                nestedArray.PushBack(i, m_document->GetAllocator());
            }
            m_document->PushBack(nestedArray.Move(), m_document->GetAllocator());
        }
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, EmptyObject)
    {
        m_document->SetObject();
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, SimpleObject)
    {
        m_document->SetObject();
        for (int i = 0; i < 5; ++i)
        {
            m_document->AddMember(CreateString(AZStd::string::format("Key%i", i)), rapidjson::Value(i), m_document->GetAllocator());
        }
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, NestedObjects)
    {
        m_document->SetObject();
        for (int j = 0; j < 7; ++j)
        {
            rapidjson::Value nestedObject(rapidjson::kObjectType);
            for (int i = 0; i < 5; ++i)
            {
                nestedObject.AddMember(CreateString(AZStd::string::format("Key%i", i)), rapidjson::Value(i), m_document->GetAllocator());
            }
            m_document->AddMember(CreateString(AZStd::string::format("Obj%i", j)), nestedObject.Move(), m_document->GetAllocator());
        }
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, Int64)
    {
        m_document->SetObject();
        AddValue("int64_min", AZStd::numeric_limits<int64_t>::min());
        AddValue("int64_max", AZStd::numeric_limits<int64_t>::max());
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, Uint64)
    {
        m_document->SetObject();
        AddValue("uint64_min", AZStd::numeric_limits<uint64_t>::min());
        AddValue("uint64_max", AZStd::numeric_limits<uint64_t>::max());
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, Double)
    {
        m_document->SetObject();
        AddValue("double_min", AZStd::numeric_limits<double>::min());
        AddValue("double_max", AZStd::numeric_limits<double>::max());
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, Null)
    {
        m_document->SetObject();
        m_document->AddMember(CreateString("null_value"), rapidjson::Value(rapidjson::kNullType), m_document->GetAllocator());
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, Bool)
    {
        m_document->SetObject();
        AddValue("true_value", true);
        AddValue("false_value", false);
        PerformSerializationChecks();
    }

    TEST_F(DomJsonTests, String)
    {
        m_document->SetObject();
        m_document->AddMember(CreateString("empty_string"), CreateString(""), m_document->GetAllocator());
        m_document->AddMember(CreateString("short_string"), CreateString("test"), m_document->GetAllocator());
        m_document->AddMember(
            CreateString("long_string"), CreateString("abcdefghijklmnopqrstuvwxyz0123456789"), m_document->GetAllocator());
        PerformSerializationChecks();
    }
} // namespace AZ::Dom::Tests
