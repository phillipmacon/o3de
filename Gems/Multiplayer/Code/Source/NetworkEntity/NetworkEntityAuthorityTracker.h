/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/EBus/Event.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzNetworking/DataStructures/TimeoutQueue.h>
#include <Source/NetworkEntity/NetworkEntityTracker.h>

namespace Multiplayer
{
    class INetworkEntityManager;

    class NetworkEntityAuthorityTracker
    {
    public:
        NetworkEntityAuthorityTracker(INetworkEntityManager& networkEntityManager);

        bool DoesEntityHaveOwner(ConstNetworkEntityHandle entityHandle) const;
        bool AddEntityAuthorityManager(ConstNetworkEntityHandle entityHandle, const HostId& newOwner);
        void RemoveEntityAuthorityManager(ConstNetworkEntityHandle entityHandle, const HostId& previousOwner);
        HostId GetEntityAuthorityManager(ConstNetworkEntityHandle entityHandle) const;

    private:
        NetworkEntityAuthorityTracker& operator= (const NetworkEntityAuthorityTracker&) = delete;

        using TimeoutDataMap = AZStd::unordered_set<NetEntityId>;
        using EntityAuthorityMap = AZStd::unordered_map<NetEntityId, AZStd::vector<HostId>>;

        TimeoutDataMap m_timeoutDataMap;
        EntityAuthorityMap m_entityAuthorityMap;
        INetworkEntityManager& m_networkEntityManager;
    };
}
