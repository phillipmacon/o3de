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

#include <TestImpactFramework/TestImpactTestSequence.h>
#include <TestImpactFramework/TestImpactRepoPath.h>

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace TestImpact
{
    //! Meta-data about the configuration. 
    struct ConfigMeta
    {
        AZStd::string m_platform; //!< The platform for which the configuration pertains to.
    };

    //! Repository configuration.
    struct RepoConfig
    {
        RepoPath m_root; //!< The absolute path to the repository root.
    };

    //! Test impact analysis framework workspace configuration.
    struct WorkspaceConfig
    {
        //! Temporary workspace configuration.
        struct Temp
        {
            //! Paths relative to root.
            struct RelativePaths
            {
                RepoPath m_artifactDirectory; //!< Path to read and write runtime artifacts to and from.
            };

            RepoPath m_root; //!< Path to the temporary workspace (cleaned prior to use).
            RelativePaths m_relativePaths;
        };

        //! Active persistent data workspace configuration.
        struct Active
        {
            //! Paths relative to root.
            struct RelativePaths
            {
                RepoPath m_sparTIAFile; //!< Path to the test impact analysis data.
                RepoPath m_enumerationCacheDirectory; //!< Path to the test enumerations cache.
            };

            RepoPath m_root; //!< Path to the persistent workspace tracked by the repository.
            RelativePaths m_relativePaths;
        };

        Temp m_temp;
        Active m_active;
    };

    //! Build target descriptor configuration.
    struct BuildTargetDescriptorConfig
    {
        RepoPath m_mappingDirectory; //!< Path to the source to target mapping files.
        AZStd::vector<AZStd::string> m_staticInclusionFilters; //!< File extensions to include for static files.
        AZStd::string m_inputOutputPairer; //!< Regex for matching autogen input files with autogen outputs files.
        AZStd::vector<AZStd::string> m_inputInclusionFilters; //!< File extensions fo include for autogen input files.
    };

    //! Dependency graph configuration.
    struct DependencyGraphDataConfig
    {
        RepoPath m_graphDirectory; //!< Path to the dependency graph files. 
        AZStd::string m_targetDependencyFileMatcher; //!< Regex for matching dependency graph files to build targets.
        AZStd::string m_targetVertexMatcher; //!< Regex form matching dependency graph vertices to build targets.
    };

    //! Test target meta configuration.
    struct TestTargetMetaConfig
    {
        RepoPath m_metaFile; //!< Path to the test target meta file.
    };

    //! Test engine configuration.
    struct TestEngineConfig
    {
        //! Test runner configuration.
        struct TestRunner
        {
            RepoPath m_binary; //!< Path to the test runner binary.
        };

        //! Test instrumentation configuration.
        struct Instrumentation
        {
            RepoPath m_binary; //!< Path to the test instrumentation binary.
        };

        TestRunner m_testRunner;
        Instrumentation m_instrumentation;
    };

    //! Build target configuration.
    struct TargetConfig
    {
        //! Test target sharding configuration.
        struct ShardedTarget
        {
            AZStd::string m_name; //!< Name of test target this sharding configuration applies to.
            ShardConfiguration m_configuration; //!< The shard configuration to use.
        };

        RepoPath m_outputDirectory; //!< Path to the test target binary directory.
        AZStd::vector<AZStd::string> m_excludedTestTargets; //!< Test targets to always exclude from test run sequences.
        AZStd::vector<ShardedTarget> m_shardedTestTargets; //!< Test target shard configurations (opt-in).
    };

    struct RuntimeConfig
    {
        ConfigMeta m_meta;
        RepoConfig m_repo;
        WorkspaceConfig m_workspace;
        BuildTargetDescriptorConfig m_buildTargetDescriptor;
        DependencyGraphDataConfig m_dependencyGraphData;
        TestTargetMetaConfig m_testTargetMeta;
        TestEngineConfig m_testEngine;
        TargetConfig m_target;
    };
} // namespace TestImpact
