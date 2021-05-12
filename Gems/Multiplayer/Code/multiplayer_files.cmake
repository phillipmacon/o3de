#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#
# For complete copyright and license terms please see the LICENSE at the root of this
# distribution (the "License"). All use of this software is governed by the License,
# or, if provided, by the license below or the license accompanying this file. Do not
# remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

set(FILES
    Include/Multiplayer/IConnectionData.h
    Include/Multiplayer/IEntityDomain.h
    Include/Multiplayer/IMultiplayer.h
    Include/Multiplayer/IMultiplayerComponentInput.h
    Include/Multiplayer/INetworkEntityManager.h
    Include/Multiplayer/INetworkTime.h
    Include/Multiplayer/IReplicationWindow.h
    Include/Multiplayer/MultiplayerComponent.h
    Include/Multiplayer/MultiplayerController.h
    Include/Multiplayer/MultiplayerComponentRegistry.h
    Include/Multiplayer/MultiplayerStats.cpp
    Include/Multiplayer/MultiplayerStats.h
    Include/Multiplayer/MultiplayerTypes.h
    Include/Multiplayer/NetBindComponent.h
	Include/Multiplayer/NetworkEntityRpcMessage.h
    Include/Multiplayer/NetworkEntityUpdateMessage.h
    Include/Multiplayer/NetworkEntityHandle.h
    Include/Multiplayer/NetworkEntityHandle.inl
    Include/Multiplayer/NetworkInput.h
	Include/Multiplayer/ReplicationRecord.h
    Include/Multiplayer/RewindableObject.h
    Include/Multiplayer/RewindableObject.inl
    Source/Multiplayer_precompiled.cpp
    Source/Multiplayer_precompiled.h
    Source/MultiplayerSystemComponent.cpp
    Source/MultiplayerSystemComponent.h
    Source/AutoGen/AutoComponent_Header.jinja
    Source/AutoGen/AutoComponent_Source.jinja
    Source/AutoGen/AutoComponent_Common.jinja
    Source/AutoGen/AutoComponentTypes_Header.jinja
    Source/AutoGen/AutoComponentTypes_Source.jinja
    Source/AutoGen/LocalPredictionPlayerInputComponent.AutoComponent.xml
    Source/AutoGen/Multiplayer.AutoPackets.xml
    Source/AutoGen/NetworkTransformComponent.AutoComponent.xml
    Source/Components/LocalPredictionPlayerInputComponent.cpp
    Source/Components/LocalPredictionPlayerInputComponent.h
    Source/Components/MultiplayerComponent.cpp
    Source/Components/MultiplayerController.cpp
	Source/Components/MultiplayerComponentRegistry.cpp
    Source/Components/NetBindComponent.cpp
    Source/Components/NetworkTransformComponent.cpp
    Source/Components/NetworkTransformComponent.h
    Source/ConnectionData/ClientToServerConnectionData.cpp
    Source/ConnectionData/ClientToServerConnectionData.h
    Source/ConnectionData/ClientToServerConnectionData.inl
    Source/ConnectionData/ServerToClientConnectionData.cpp
    Source/ConnectionData/ServerToClientConnectionData.h
    Source/ConnectionData/ServerToClientConnectionData.inl
    Source/EntityDomains/FullOwnershipEntityDomain.cpp
    Source/EntityDomains/FullOwnershipEntityDomain.h
    Source/NetworkEntity/EntityReplication/EntityReplicationManager.cpp
    Source/NetworkEntity/EntityReplication/EntityReplicationManager.h
    Source/NetworkEntity/EntityReplication/EntityReplicator.cpp
    Source/NetworkEntity/EntityReplication/EntityReplicator.h
    Source/NetworkEntity/EntityReplication/EntityReplicator.inl
    Source/NetworkEntity/EntityReplication/PropertyPublisher.cpp
    Source/NetworkEntity/EntityReplication/PropertyPublisher.h
    Source/NetworkEntity/EntityReplication/PropertySubscriber.cpp
    Source/NetworkEntity/EntityReplication/PropertySubscriber.h
    Source/NetworkEntity/EntityReplication/ReplicationRecord.cpp
    Source/NetworkEntity/NetworkEntityAuthorityTracker.cpp
    Source/NetworkEntity/NetworkEntityAuthorityTracker.h
    Source/NetworkEntity/NetworkEntityHandle.cpp
    Source/NetworkEntity/NetworkEntityManager.cpp
    Source/NetworkEntity/NetworkEntityManager.h
    Source/NetworkEntity/NetworkSpawnableLibrary.cpp
    Source/NetworkEntity/NetworkSpawnableLibrary.h
    Source/NetworkEntity/NetworkEntityRpcMessage.cpp
    Source/NetworkEntity/NetworkEntityTracker.cpp
    Source/NetworkEntity/NetworkEntityTracker.h
    Source/NetworkEntity/NetworkEntityTracker.inl
    Source/NetworkEntity/NetworkEntityUpdateMessage.cpp
    Source/NetworkInput/NetworkInput.cpp
    Source/NetworkInput/NetworkInputArray.cpp
    Source/NetworkInput/NetworkInputArray.h
    Source/NetworkInput/NetworkInputChild.cpp
    Source/NetworkInput/NetworkInputChild.h
    Source/NetworkInput/NetworkInputHistory.cpp
    Source/NetworkInput/NetworkInputHistory.h
    Source/NetworkInput/NetworkInputMigrationVector.cpp
    Source/NetworkInput/NetworkInputMigrationVector.h
    Source/NetworkTime/NetworkTime.cpp
    Source/NetworkTime/NetworkTime.h
    Source/Pipeline/NetBindMarkerComponent.cpp
    Source/Pipeline/NetBindMarkerComponent.h
    Source/Pipeline/NetworkSpawnableHolderComponent.cpp
    Source/Pipeline/NetworkSpawnableHolderComponent.h
    Source/ReplicationWindows/NullReplicationWindow.cpp
    Source/ReplicationWindows/NullReplicationWindow.h
    Source/ReplicationWindows/ServerToClientReplicationWindow.cpp
    Source/ReplicationWindows/ServerToClientReplicationWindow.h
)
