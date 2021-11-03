/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Source/NetworkEntity/NetworkEntityAuthorityTracker.h>
#include <Multiplayer/Components/NetBindComponent.h>
#include <Multiplayer/NetworkEntity/INetworkEntityManager.h>
#include <AzCore/Console/IConsole.h>
#include <AzCore/Console/ILogger.h>
#include <AzCore/EBus/IEventScheduler.h>
#include <AzNetworking/Utilities/NetworkCommon.h>
#include <AzNetworking/Serialization/NetworkOutputSerializer.h>

namespace Multiplayer
{
    AZ_CVAR(AZ::TimeMs, net_EntityMigrationTimeoutMs, AZ::TimeMs{ 1000 }, nullptr, AZ::ConsoleFunctorFlags::Null, "Time to wait for a new authority to attach to an entity before we delete the entity");

    NetworkEntityAuthorityTracker::NetworkEntityAuthorityTracker(INetworkEntityManager& networkEntityManager)
        : m_networkEntityManager(networkEntityManager)
    {
        ;
    }

    bool NetworkEntityAuthorityTracker::AddEntityAuthorityManager(ConstNetworkEntityHandle entityHandle, const HostId& newOwner)
    {
        bool ret = false;
        auto timeoutData = m_timeoutDataMap.find(entityHandle.GetNetEntityId());
        if (timeoutData != m_timeoutDataMap.end())
        {
            AZLOG
            (
                NET_AuthTracker,
                "AuthTracker: Removing timeout for networkEntityId %llu, new owner is %s",
                aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()),
                newOwner.GetString().c_str()
            );
            m_timeoutDataMap.erase(timeoutData);
            ret = true;
        }

        AZLOG
        (
            NET_AuthTracker,
            "AuthTracker: Assigning networkEntityId %llu to %s",
            aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()),
            newOwner.GetString().c_str()
        );

        m_entityAuthorityMap[entityHandle.GetNetEntityId()].push_back(newOwner);
        return ret;
    }

    void NetworkEntityAuthorityTracker::RemoveEntityAuthorityManager(ConstNetworkEntityHandle entityHandle, const HostId& previousOwner)
    {
        auto mapIter = m_entityAuthorityMap.find(entityHandle.GetNetEntityId());
        if (mapIter != m_entityAuthorityMap.end())
        {
            auto& authorityStack = mapIter->second;
            for (auto stackIter = authorityStack.begin(); stackIter != authorityStack.end();)
            {
                if (*stackIter == previousOwner)
                {
                    stackIter = authorityStack.erase(stackIter);
                }
                else
                {
                    ++stackIter;
                }
            }

            AZLOG(NET_AuthTracker, "AuthTracker: Removing networkEntityId %llu from %s", aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()), previousOwner.GetString().c_str());
            if (auto localEnt = entityHandle.GetEntity())
            {
                if (authorityStack.empty())
                {
                    m_entityAuthorityMap.erase(entityHandle.GetNetEntityId());
                    NetEntityRole networkRole = NetEntityRole::InvalidRole;
                    NetBindComponent* netBindComponent = entityHandle.GetNetBindComponent();
                    if (netBindComponent != nullptr)
                    {
                        networkRole = netBindComponent->GetNetEntityRole();
                    }
                    if (networkRole != NetEntityRole::Autonomous)
                    {
                        AZ_Assert
                        (
                            m_timeoutDataMap.find(entityHandle.GetNetEntityId()) == m_timeoutDataMap.end(),
                            "Trying to add something twice to the timeout map, this is unexpected"
                        );
                        m_timeoutDataMap.insert(entityHandle.GetNetEntityId());
                        AZ::Interface<AZ::IEventScheduler>::Get()->AddCallback([this, netEntityId = entityHandle.GetNetEntityId(), previousOwner]
                            {
                                auto timeoutData = m_timeoutDataMap.find(netEntityId);
                                if (timeoutData != m_timeoutDataMap.end())
                                {
                                    m_timeoutDataMap.erase(timeoutData);
                                    ConstNetworkEntityHandle entityHandle = m_networkEntityManager.GetEntity(netEntityId);
                                    if (auto entity = entityHandle.GetEntity())
                                    {
                                        NetEntityRole networkRole = NetEntityRole::InvalidRole;
                                        NetBindComponent* netBindComponent = entityHandle.GetNetBindComponent();
                                        if (netBindComponent != nullptr)
                                        {
                                            networkRole = netBindComponent->GetNetEntityRole();
                                        }
                                        if (networkRole != NetEntityRole::Authority)
                                        {
                                            AZLOG_ERROR
                                            (
                                                "Timed out entity id %llu during migration previous owner %s, removing it",
                                                aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()),
                                                previousOwner.GetString().c_str()
                                            );
                                            m_networkEntityManager.MarkForRemoval(entityHandle);
                                        }
                                    }
                                }
                            },
                            AZ::Name("Entity authority removal functor"),
                            net_EntityMigrationTimeoutMs
                        );
                    }
                    else
                    {
                        AZLOG(NET_AuthTracker, "AuthTracker: Skipping timeout for Autonomous networkEntityId %llu", aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()));
                    }
                }
            }
        }
        else
        {
            AZLOG(NET_AuthTracker, "AuthTracker: Remove authority called on networkEntityId that was never added %llu", aznumeric_cast<AZ::u64>(entityHandle.GetNetEntityId()));
            AZ_Assert(false, "AuthTracker: Remove authority called on entity that was never added");
        }
    }

    HostId NetworkEntityAuthorityTracker::GetEntityAuthorityManager(ConstNetworkEntityHandle entityHandle) const
    {
        if (auto localEnt = entityHandle.GetEntity())
        {
            NetEntityRole networkRole = NetEntityRole::InvalidRole;
            NetBindComponent* netBindComponent = entityHandle.GetNetBindComponent();
            if (netBindComponent != nullptr)
            {
                networkRole = netBindComponent->GetNetEntityRole();
            }
            if (networkRole == NetEntityRole::Authority)
            {
                return m_networkEntityManager.GetHostId();
            }
            else
            {
                auto iter = m_entityAuthorityMap.find(entityHandle.GetNetEntityId());
                if (iter != m_entityAuthorityMap.end())
                {
                    if (!iter->second.empty())
                    {
                        return iter->second.back();
                    }
                }
            }
        }
        return InvalidHostId;
    }

    bool NetworkEntityAuthorityTracker::DoesEntityHaveOwner(ConstNetworkEntityHandle entityHandle) const
    {
        return InvalidHostId != GetEntityAuthorityManager(entityHandle);
    }
}
