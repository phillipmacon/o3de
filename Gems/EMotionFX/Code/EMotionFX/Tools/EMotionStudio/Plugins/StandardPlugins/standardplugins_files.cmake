#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Source/StandardPluginsConfig.h
    Source/ActionHistory/ActionHistoryCallback.cpp
    Source/ActionHistory/ActionHistoryCallback.h
    Source/ActionHistory/ActionHistoryPlugin.cpp
    Source/ActionHistory/ActionHistoryPlugin.h
    Source/Attachments/AttachmentNodesWindow.cpp
    Source/Attachments/AttachmentNodesWindow.h
    Source/Attachments/AttachmentsHierarchyWindow.cpp
    Source/Attachments/AttachmentsHierarchyWindow.h
    Source/Attachments/AttachmentsPlugin.cpp
    Source/Attachments/AttachmentsPlugin.h
    Source/Attachments/AttachmentsWindow.cpp
    Source/Attachments/AttachmentsWindow.h
    Source/AnimGraph/AnimGraphActionManager.cpp
    Source/AnimGraph/AnimGraphActionManager.h
    Source/AnimGraph/AnimGraphEditor.cpp
    Source/AnimGraph/AnimGraphEditor.h
    Source/AnimGraph/AnimGraphItemDelegate.cpp
    Source/AnimGraph/AnimGraphItemDelegate.h
    Source/AnimGraph/AnimGraphModelCallbacks.cpp
    Source/AnimGraph/AnimGraphModel.cpp
    Source/AnimGraph/AnimGraphModel.h
    Source/AnimGraph/AnimGraphSortFilterProxyModel.cpp
    Source/AnimGraph/AnimGraphSortFilterProxyModel.h
    Source/AnimGraph/AttributesWindow.cpp
    Source/AnimGraph/AttributesWindow.h
    Source/AnimGraph/BlendTreeVisualNode.cpp
    Source/AnimGraph/BlendTreeVisualNode.h
    Source/AnimGraph/BlendGraphViewWidget.cpp
    Source/AnimGraph/BlendGraphViewWidget.h
    Source/AnimGraph/BlendGraphWidget.cpp
    Source/AnimGraph/BlendGraphWidget.h
    Source/AnimGraph/BlendNodeSelectionWindow.cpp
    Source/AnimGraph/BlendNodeSelectionWindow.h
    Source/AnimGraph/BlendSpaceNodeWidget.cpp
    Source/AnimGraph/BlendSpaceNodeWidget.h
    Source/AnimGraph/BlendSpace1DNodeWidget.cpp
    Source/AnimGraph/BlendSpace1DNodeWidget.h
    Source/AnimGraph/BlendSpace2DNodeWidget.cpp
    Source/AnimGraph/BlendSpace2DNodeWidget.h
    Source/AnimGraph/AnimGraphVisualNode.cpp
    Source/AnimGraph/AnimGraphVisualNode.h
    Source/AnimGraph/AnimGraphHierarchyWidget.cpp
    Source/AnimGraph/AnimGraphHierarchyWidget.h
    Source/AnimGraph/AnimGraphNodeWidget.h
    Source/AnimGraph/AnimGraphNodeWidget.cpp
    Source/AnimGraph/AnimGraphOptions.cpp
    Source/AnimGraph/AnimGraphOptions.h
    Source/AnimGraph/AnimGraphPlugin.cpp
    Source/AnimGraph/AnimGraphPlugin.h
    Source/AnimGraph/AnimGraphPluginCallbacks.cpp
    Source/AnimGraph/ContextMenu.cpp
    Source/AnimGraph/DebugEventHandler.cpp
    Source/AnimGraph/DebugEventHandler.h
    Source/AnimGraph/GameController.cpp
    Source/AnimGraph/GameController.h
    Source/AnimGraph/GameControllerWindow.cpp
    Source/AnimGraph/GameControllerWindow.h
    Source/AnimGraph/GraphNode.cpp
    Source/AnimGraph/GraphNode.h
    Source/AnimGraph/GraphNodeFactory.cpp
    Source/AnimGraph/GraphNodeFactory.h
    Source/AnimGraph/NavigateWidget.cpp
    Source/AnimGraph/NavigateWidget.h
    Source/AnimGraph/NavigationHistory.cpp
    Source/AnimGraph/NavigationHistory.h
    Source/AnimGraph/NavigationLinkWidget.cpp
    Source/AnimGraph/NavigationLinkWidget.h
    Source/AnimGraph/NodeConnection.cpp
    Source/AnimGraph/NodeConnection.h
    Source/AnimGraph/NodeGraph.cpp
    Source/AnimGraph/NodeGraph.h
    Source/AnimGraph/NodeGraphWidget.cpp
    Source/AnimGraph/NodeGraphWidget.h
    Source/AnimGraph/NodeGroupWindow.cpp
    Source/AnimGraph/NodeGroupWindow.h
    Source/AnimGraph/NodePaletteWidget.cpp
    Source/AnimGraph/NodePaletteWidget.h
    Source/AnimGraph/ParameterCreateEditDialog.cpp
    Source/AnimGraph/ParameterCreateEditDialog.h
    Source/AnimGraph/ParameterSelectionWindow.cpp
    Source/AnimGraph/ParameterSelectionWindow.h
    Source/AnimGraph/ParameterWidget.cpp
    Source/AnimGraph/ParameterWidget.h
    Source/AnimGraph/ParameterWindow.cpp
    Source/AnimGraph/ParameterWindow.h
    Source/AnimGraph/RoleFilterProxyModel.cpp
    Source/AnimGraph/RoleFilterProxyModel.h
    Source/AnimGraph/AnimGraphSelectionProxyModel.cpp
    Source/AnimGraph/SelectionProxyModel.h
    Source/AnimGraph/StateFilterSelectionWindow.cpp
    Source/AnimGraph/StateFilterSelectionWindow.h
    Source/AnimGraph/StateGraphNode.cpp
    Source/AnimGraph/StateGraphNode.h
    Source/AnimGraph/ParameterEditor/BoolParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/BoolParameterEditor.h
    Source/AnimGraph/ParameterEditor/ColorParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/ColorParameterEditor.h
    Source/AnimGraph/ParameterEditor/FloatSliderParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/FloatSliderParameterEditor.h
    Source/AnimGraph/ParameterEditor/FloatSpinnerParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/FloatSpinnerParameterEditor.h
    Source/AnimGraph/ParameterEditor/IntSliderParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/IntSliderParameterEditor.h
    Source/AnimGraph/ParameterEditor/IntSpinnerParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/IntSpinnerParameterEditor.h
    Source/AnimGraph/ParameterEditor/ParameterEditorFactory.cpp
    Source/AnimGraph/ParameterEditor/ParameterEditorFactory.h
    Source/AnimGraph/ParameterEditor/RotationParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/RotationParameterEditor.h
    Source/AnimGraph/ParameterEditor/StringParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/StringParameterEditor.h
    Source/AnimGraph/ParameterEditor/TagParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/TagParameterEditor.h
    Source/AnimGraph/ParameterEditor/ValueParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/ValueParameterEditor.h
    Source/AnimGraph/ParameterEditor/Vector2ParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/Vector2ParameterEditor.h
    Source/AnimGraph/ParameterEditor/Vector3GizmoParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/Vector3GizmoParameterEditor.h
    Source/AnimGraph/ParameterEditor/Vector3ParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/Vector3ParameterEditor.h
    Source/AnimGraph/ParameterEditor/Vector4ParameterEditor.cpp
    Source/AnimGraph/ParameterEditor/Vector4ParameterEditor.h
    Source/CommandBar/CommandBarPlugin.cpp
    Source/CommandBar/CommandBarPlugin.h
    Source/CommandBrowser/CommandBrowserPlugin.cpp
    Source/CommandBrowser/CommandBrowserPlugin.h
    Source/LogWindow/LogWindowCallback.cpp
    Source/LogWindow/LogWindowCallback.h
    Source/LogWindow/LogWindowPlugin.cpp
    Source/LogWindow/LogWindowPlugin.h
    Source/MorphTargetsWindow/MorphTargetEditWindow.cpp
    Source/MorphTargetsWindow/MorphTargetEditWindow.h
    Source/MorphTargetsWindow/MorphTargetGroupWidget.cpp
    Source/MorphTargetsWindow/MorphTargetGroupWidget.h
    Source/MorphTargetsWindow/MorphTargetsWindowPlugin.cpp
    Source/MorphTargetsWindow/MorphTargetsWindowPlugin.h
    Source/MorphTargetsWindow/PhonemeSelectionWindow.cpp
    Source/MorphTargetsWindow/PhonemeSelectionWindow.h
    Source/MotionEvents/EventDataEditor.cpp
    Source/MotionEvents/EventDataEditor.h
    Source/MotionEvents/MotionEventEditor.cpp
    Source/MotionEvents/MotionEventEditor.h
    Source/MotionEvents/MotionEventPresetCreateDialog.cpp
    Source/MotionEvents/MotionEventPresetCreateDialog.h
    Source/MotionEvents/MotionEventPresetsWidget.cpp
    Source/MotionEvents/MotionEventPresetsWidget.h
    Source/MotionEvents/MotionEventWidget.cpp
    Source/MotionEvents/MotionEventWidget.h
    Source/MotionEvents/MotionEventsPlugin.cpp
    Source/MotionEvents/MotionEventsPlugin.h
    Source/MotionSetsWindow/MotionSetManagementWindow.cpp
    Source/MotionSetsWindow/MotionSetManagementWindow.h
    Source/MotionSetsWindow/MotionSetsWindowPlugin.cpp
    Source/MotionSetsWindow/MotionSetsWindowPlugin.h
    Source/MotionSetsWindow/MotionSetWindow.cpp
    Source/MotionSetsWindow/MotionSetWindow.h
    Source/MotionWindow/MotionExtractionWindow.cpp
    Source/MotionWindow/MotionExtractionWindow.h
    Source/MotionWindow/MotionListWindow.cpp
    Source/MotionWindow/MotionListWindow.h
    Source/MotionWindow/MotionPropertiesWindow.cpp
    Source/MotionWindow/MotionPropertiesWindow.h
    Source/MotionWindow/MotionRetargetingWindow.cpp
    Source/MotionWindow/MotionRetargetingWindow.h
    Source/MotionWindow/MotionWindowPlugin.cpp
    Source/MotionWindow/MotionWindowPlugin.h
    Source/NodeGroups/NodeGroupManagementWidget.cpp
    Source/NodeGroups/NodeGroupManagementWidget.h
    Source/NodeGroups/NodeGroupsPlugin.cpp
    Source/NodeGroups/NodeGroupsPlugin.h
    Source/NodeGroups/NodeGroupWidget.cpp
    Source/NodeGroups/NodeGroupWidget.h
    Source/NodeWindow/ActorInfo.cpp
    Source/NodeWindow/ActorInfo.h
    Source/NodeWindow/MeshInfo.cpp
    Source/NodeWindow/MeshInfo.h
    Source/NodeWindow/NamedPropertyStringValue.cpp
    Source/NodeWindow/NamedPropertyStringValue.h
    Source/NodeWindow/NodeInfo.cpp
    Source/NodeWindow/NodeInfo.h
    Source/NodeWindow/NodeGroupInfo.cpp
    Source/NodeWindow/NodeGroupInfo.h
    Source/NodeWindow/NodeWindowPlugin.cpp
    Source/NodeWindow/NodeWindowPlugin.h
    Source/NodeWindow/SubMeshInfo.cpp
    Source/NodeWindow/SubMeshInfo.h
    Source/SceneManager/ActorPropertiesWindow.cpp
    Source/SceneManager/ActorPropertiesWindow.h
    Source/SceneManager/ActorsWindow.cpp
    Source/SceneManager/ActorsWindow.h
    Source/SceneManager/MirrorSetupWindow.cpp
    Source/SceneManager/MirrorSetupWindow.h
    Source/SceneManager/SceneManagerPlugin.cpp
    Source/SceneManager/SceneManagerPlugin.h
    Source/TimeView/PlaybackControlsGroup.cpp
    Source/TimeView/PlaybackControlsGroup.h
    Source/TimeView/PlaybackOptionsGroup.cpp
    Source/TimeView/PlaybackOptionsGroup.h
    Source/TimeView/RecorderGroup.cpp
    Source/TimeView/RecorderGroup.h
    Source/TimeView/TimeInfoWidget.cpp
    Source/TimeView/TimeInfoWidget.h
    Source/TimeView/TimeTrack.cpp
    Source/TimeView/TimeTrack.h
    Source/TimeView/TimeTrackElement.cpp
    Source/TimeView/TimeTrackElement.h
    Source/TimeView/TimeViewPlugin.cpp
    Source/TimeView/TimeViewPlugin.h
    Source/TimeView/TimeViewShared.h
    Source/TimeView/TimeViewToolBar.cpp
    Source/TimeView/TimeViewToolBar.h
    Source/TimeView/TrackDataHeaderWidget.cpp
    Source/TimeView/TrackDataHeaderWidget.h
    Source/TimeView/TrackDataWidget.cpp
    Source/TimeView/TrackDataWidget.h
    Source/TimeView/TrackHeaderWidget.cpp
    Source/TimeView/TrackHeaderWidget.h
)
