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
    Include/IMultiplayer.h
    Source/Multiplayer_precompiled.cpp
    Source/Multiplayer_precompiled.h
    Source/MultiplayerSystemComponent.cpp
    Source/MultiplayerSystemComponent.h
    Source/MultiplayerTypes.h
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
    Source/Components/MultiplayerComponent.h
    Source/Components/MultiplayerController.cpp
    Source/Components/MultiplayerController.h
    Source/Components/NetBindComponent.cpp
    Source/Components/NetBindComponent.h
    Source/Components/NetworkTransformComponent.cpp
    Source/Components/NetworkTransformComponent.h
    Source/ConnectionData/IConnectionData.h
    Source/ConnectionData/ServerToClientConnectionData.cpp
    Source/ConnectionData/ServerToClientConnectionData.h
    Source/ConnectionData/ServerToClientConnectionData.inl
    Source/EntityDomains/FullOwnershipEntityDomain.cpp
    Source/EntityDomains/FullOwnershipEntityDomain.h
    Source/EntityDomains/IEntityDomain.h
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
    Source/NetworkEntity/EntityReplication/ReplicationRecord.h
    Source/NetworkEntity/INetworkEntityManager.h
    Source/NetworkEntity/NetworkEntityAuthorityTracker.cpp
    Source/NetworkEntity/NetworkEntityAuthorityTracker.h
    Source/NetworkEntity/NetworkEntityHandle.cpp
    Source/NetworkEntity/NetworkEntityHandle.h
    Source/NetworkEntity/NetworkEntityHandle.inl
    Source/NetworkEntity/NetworkEntityManager.cpp
    Source/NetworkEntity/NetworkEntityManager.h
    Source/NetworkEntity/NetworkEntityRpcMessage.cpp
    Source/NetworkEntity/NetworkEntityRpcMessage.h
    Source/NetworkEntity/NetworkEntityTracker.cpp
    Source/NetworkEntity/NetworkEntityTracker.h
    Source/NetworkEntity/NetworkEntityTracker.inl
    Source/NetworkEntity/NetworkEntityUpdateMessage.cpp
    Source/NetworkEntity/NetworkEntityUpdateMessage.h
    Source/NetworkInput/IMultiplayerComponentInput.h
    Source/NetworkInput/NetworkInput.cpp
    Source/NetworkInput/NetworkInput.h
    Source/NetworkInput/NetworkInputChild.cpp
    Source/NetworkInput/NetworkInputChild.h
    Source/NetworkInput/NetworkInputHistory.cpp
    Source/NetworkInput/NetworkInputHistory.h
    Source/NetworkInput/NetworkInputVector.cpp
    Source/NetworkInput/NetworkInputVector.h
    Source/NetworkTime/INetworkTime.h
    Source/NetworkTime/NetworkTime.cpp
    Source/NetworkTime/NetworkTime.h
    Source/NetworkTime/RewindableObject.h
    Source/NetworkTime/RewindableObject.inl
    Source/ReplicationWindows/IReplicationWindow.h
    Source/ReplicationWindows/ServerToClientReplicationWindow.cpp
    Source/ReplicationWindows/ServerToClientReplicationWindow.h
)
