#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Tests/ActorBusTests.cpp
    Tests/ActorComponentBusTests.cpp
    Tests/ActorFixture.cpp
    Tests/ActorFixture.h
    Tests/ActorInstanceCommandTests.cpp
    Tests/AdditiveMotionSamplingTests.cpp
    Tests/AnimAudioComponentTests.cpp
    Tests/AnimGraphActionTests.cpp
    Tests/AnimGraphCommandTests.cpp
    Tests/AnimGraphActionCommandTests.cpp
    Tests/AnimGraphActionTests.cpp
    Tests/AnimGraphComponentBusTests.cpp
    Tests/AnimGraphNetworkingBusTests.cpp
    Tests/AnimGraphCopyPasteTests.cpp
    Tests/AnimGraphDeferredInitTests.cpp
    Tests/AnimGraphEventHandlerCounter.h
    Tests/AnimGraphEventHandlerCounter.cpp
    Tests/AnimGraphEventTests.cpp
    Tests/AnimGraphFixture.cpp
    Tests/AnimGraphFixture.h
    Tests/AnimGraphFuzzTests.cpp
    Tests/AnimGraphHubNodeTests.cpp
    Tests/AnimGraphLoadingTests.cpp
    Tests/AnimGraphMotionConditionTests.cpp
    Tests/AnimGraphMotionNodeTests.cpp
    Tests/AnimGraphNodeEventFilterTests.cpp
    Tests/AnimGraphNodeGroupTests.cpp
    Tests/AnimGraphNodeProcessingTests.cpp
    Tests/AnimGraphParameterActionTests.cpp
    Tests/AnimGraphParameterActionTests.cpp
    Tests/AnimGraphParameterConditionCommandTests.cpp
    Tests/AnimGraphParameterConditionTests.cpp
    Tests/AnimGraphParameterConditionCommandTests.cpp
    Tests/AnimGraphRefCountTests.cpp
    Tests/AnimGraphReferenceNodeTests.cpp
    Tests/AnimGraphStateMachineTests.cpp
    Tests/AnimGraphStateMachineInterruptionTests.cpp
    Tests/AnimGraphStateMachineSyncTests.cpp
    Tests/AnimGraphSyncTrackTests.cpp
    Tests/AnimGraphTagConditionTests.cpp
    Tests/AnimGraphTransitionCommandTests.cpp
    Tests/AnimGraphTransitionConditionCommandTests.cpp
    Tests/AnimGraphTransitionConditionFixture.cpp
    Tests/AnimGraphTransitionConditionFixture.h
    Tests/AnimGraphTransitionConditionTests.cpp
    Tests/AnimGraphTransitionFixture.cpp
    Tests/AnimGraphTransitionFixture.h
    Tests/AnimGraphTransitionTests.cpp
    Tests/AnimGraphVector2ConditionTests.cpp
    Tests/AutoSkeletonLODTests.cpp
    Tests/BlendSpaceFixture.h
    Tests/BlendSpaceFixture.cpp
    Tests/BlendSpaceTests.cpp
    Tests/BlendTreeBlendNNodeTests.cpp
    Tests/BlendTreeFloatConstantNodeTests.cpp
    Tests/BlendTreeFloatConditionNodeTests.cpp
    Tests/BlendTreeFloatMath1NodeTests.cpp
    Tests/BlendTreeFootIKNodeTests.cpp
    Tests/BlendTreeMaskNodeTests.cpp
    Tests/BlendTreeMirrorPoseNodeTests.cpp
    Tests/BlendTreeMotionFrameNodeTests.cpp
    Tests/BlendTreeRagdollNodeTests.cpp
    Tests/BlendTreeRangeRemapperNodeTests.cpp
    Tests/BlendTreeRotationMath2NodeTests.cpp
    Tests/BlendTreeRotationLimitNodeTests.cpp
    Tests/BlendTreeSimulatedObjectNodeTests.cpp
    Tests/BlendTreeTransformNodeTests.cpp
    Tests/BlendTreeTwoLinkIKNodeTests.cpp
    Tests/BoolLogicNodeTests.cpp
    Tests/ColliderCommandTests.cpp
    Tests/EMotionFXTest.cpp
    Tests/EmotionFXMathLibTests.cpp
    Tests/EventManagerTests.cpp
    Tests/JackGraphFixture.h
    Tests/JackGraphFixture.cpp
    Tests/KeyTrackLinearTests.cpp
    Tests/LeaderFollowerVersionTests.cpp
    Tests/MCore/Array2DTests.cpp
    Tests/MCoreSystemFixture.h
    Tests/MCoreSystemFixture.cpp
    Tests/MorphTargetRuntimeTests.cpp
    Tests/MorphSkinAttachmentTests.cpp
    Tests/MotionEventCommandTests.cpp
    Tests/MotionEventTrackTests.cpp
    Tests/MotionExtractionTests.cpp
    Tests/MotionExtractionBusTests.cpp
    Tests/MotionInstanceTests.cpp
    Tests/MotionLayerSystemTests.cpp
    Tests/MultiThreadSchedulerTests.cpp
    Tests/PoseTests.cpp
    Tests/Printers.cpp
    Tests/QuaternionParameterTests.cpp
    Tests/RagdollCommandTests.cpp
    Tests/RandomMotionSelectionTests.cpp
    Tests/RenderBackendManagerTests.cpp
    Tests/SelectionListTests.cpp
    Tests/SimpleMotionComponentBusTests.cpp
    Tests/SimulatedObjectCommandTests.cpp
    Tests/SimulatedObjectSerializeTests.cpp
    Tests/SkeletalLODTests.cpp
    Tests/SkeletonNodeSearchTests.cpp
    Tests/SyncingSystemTests.cpp
    Tests/SystemComponentFixture.h
    Tests/SystemComponentTests.cpp
    Tests/TransformUnitTests.cpp
    Tests/Vector2ToVector3CompatibilityTests.cpp
    Tests/Vector3ParameterTests.cpp
    Tests/PhysicsSetupUtils.h
    Tests/PhysicsSetupUtils.cpp
    Tests/MCore/CommandManagerTests.cpp
    Tests/MCore/CommandLineTests.cpp
    Tests/Prefabs/LeftArmSkeleton.h
    Tests/TestAssetCode/MotionEvent.h
    Tests/TestAssetCode/MotionEvent.cpp
    Tests/TestAssetCode/TestActorAssets.h
    Tests/TestAssetCode/TestActorAssets.cpp
    Tests/TestAssetCode/TestMotionAssets.h
    Tests/TestAssetCode/TestMotionAssets.cpp
    Tests/Mocks/Actor.h
    Tests/Mocks/ActorManager.h
    Tests/Mocks/AnimGraph.h
    Tests/Mocks/AnimGraphInstance.h
    Tests/Mocks/AnimGraphManager.h
    Tests/Mocks/AnimGraphNode.h
    Tests/Mocks/AnimGraphObject.h
    Tests/Mocks/AnimGraphObjectData.h
    Tests/Mocks/AnimGraphStateTransition.h
    Tests/Mocks/BlendTreeParameterNode.h
    Tests/Mocks/Command.h
    Tests/Mocks/CommandManager.h
    Tests/Mocks/CommandManagerCallback.h
    Tests/Mocks/CommandSystemCommandManager.h
    Tests/Mocks/EMotionFXManager.h
    Tests/Mocks/GroupParameter.h
    Tests/Mocks/Node.h
    Tests/Mocks/ObjectAffectedByParameterChanges.h
    Tests/Mocks/Parameter.h
    Tests/Mocks/ParameterFactory.h
    Tests/Mocks/PhysicsRagdoll.h
    Tests/Mocks/SimulatedObject.h
    Tests/Mocks/SimulatedObjectSetup.h
    Tests/Mocks/Skeleton.h
    Tests/Mocks/ValueParameter.h
    Tests/Integration/PoseComparisonFixture.h
    Tests/Integration/PoseComparisonTests.cpp
    Tests/Integration/EntityComponentFixture.h
    Tests/Integration/EntityComponentFixture.cpp
    Tests/Integration/ActorComponentRagdollTests.cpp
    Tests/Integration/ActorComponentAttachmentTest.cpp
    Tests/Integration/Components/AnimGraph/CanApplyDefaultParameterValues.cpp
    Tests/Game/SampleGameFixture.h
    Tests/Game/SamplePerformanceTests.cpp
    Tests/Bugs/CanDeleteExitNodeAfterItHasBeenActive.cpp
    Tests/AnimGraphParameterCommandsTests.cpp
    Tests/CommandAdjustSimulatedObjectTests.cpp
    Tests/SimulatedObjectSetupTests.cpp
)

# The following file wraps existing headers around a file specific namespace, causing any 
# file that uses any of the symbols in those files to fail because it wont find the symbol
# due to it being hijacked
set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Tests/CommandAdjustSimulatedObjectTests.cpp
)
