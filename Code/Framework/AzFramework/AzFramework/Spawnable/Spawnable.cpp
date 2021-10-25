/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/sort.h>
#include <AzFramework/Spawnable/Spawnable.h>

namespace AzFramework
{
    //
    // EntityAlias
    //


    bool Spawnable::EntityAlias::HasLowerIndex(const EntityAlias& other) const
    {
        return m_sourceIndex == other.m_sourceIndex ?
            m_aliasType < other.m_aliasType :
            m_sourceIndex < other.m_sourceIndex;
    }


    //
    // EntityAliasVisitorBase
    //

    bool Spawnable::EntityAliasVisitorBase::HasLock(const EntityAliasList* aliases) const
    {
        return aliases != nullptr;
    }

    bool Spawnable::EntityAliasVisitorBase::HasAliases(const EntityAliasList* aliases) const
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        return !aliases->empty();
    }

    bool Spawnable::EntityAliasVisitorBase::AreAllSpawnablesReady(const EntityAliasList* aliases) const
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        for (const EntityAlias& alias : *aliases)
        {
            if ((alias.m_aliasType != Spawnable::EntityAliasType::Original && alias.m_aliasType != Spawnable::EntityAliasType::Disabled) &&
                !alias.m_spawnable.IsReady())
            {
                return false;
            }
        }
        return true;
    }

    auto Spawnable::EntityAliasVisitorBase::begin(const EntityAliasList* aliases) const -> EntityAliasList::const_iterator
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        return aliases->cbegin();
    }

    auto Spawnable::EntityAliasVisitorBase::end(const EntityAliasList* aliases) const -> EntityAliasList::const_iterator
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        return aliases->cend();
    }

    auto Spawnable::EntityAliasVisitorBase::cbegin(const EntityAliasList* aliases) const -> EntityAliasList::const_iterator
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        return aliases->cbegin();
    }

    auto Spawnable::EntityAliasVisitorBase::cend(const EntityAliasList* aliases) const -> EntityAliasList::const_iterator
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        return aliases->cend();
    }

    void Spawnable::EntityAliasVisitorBase::ListTargetSpawnables(
        const EntityAliasList* aliases, const ListTargetSpawanblesCallback& callback) const
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        AZStd::unordered_set<AZ::Data::AssetId> spawnableIds;
        for (const Spawnable::EntityAlias& alias : *aliases)
        {
            auto it = spawnableIds.find(alias.m_spawnable.GetId());
            if (it == spawnableIds.end())
            {
                callback(alias.m_spawnable);
                spawnableIds.emplace(alias.m_spawnable.GetId());
            }
        }
    }

    void Spawnable::EntityAliasVisitorBase::ListTargetSpawnables(
        const EntityAliasList* aliases, AZ::Crc32 tag, const ListTargetSpawanblesCallback& callback) const
    {
        AZ_Assert(aliases, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        AZStd::unordered_set<AZ::Data::AssetId> spawnableIds;
        for (const Spawnable::EntityAlias& alias : *aliases)
        {
            if (alias.m_tag == tag)
            {
                auto it = spawnableIds.find(alias.m_spawnable.GetId());
                if (it == spawnableIds.end())
                {
                    callback(alias.m_spawnable);
                    spawnableIds.emplace(alias.m_spawnable.GetId());
                }
            }
        }
    }


    //
    // EntityAliasVisitor
    //


    Spawnable::EntityAliasVisitor::EntityAliasVisitor(Spawnable& owner, EntityAliasList* entityAliasList)
        : m_owner(owner)
        , m_entityAliasList(entityAliasList)
    {
    }

    Spawnable::EntityAliasVisitor::~EntityAliasVisitor()
    {
        if (HasLock())
        {
            Optimize();

            AZ_Assert(
                m_owner.m_lockState == LockState::Locked, "Attempting to unlock a spawnable that's not in the locked state (%i).",
                m_owner.m_lockState.load());
            m_owner.m_lockState = LockState::Unlocked;
        }
    }

    Spawnable::EntityAliasVisitor::EntityAliasVisitor(EntityAliasVisitor&& rhs)
        : m_owner(rhs.m_owner)
        , m_entityAliasList(rhs.m_entityAliasList)
    {
        m_dirty = rhs.m_dirty;

        rhs.m_entityAliasList = nullptr;
        rhs.m_dirty = false;
    }

    auto Spawnable::EntityAliasVisitor::operator=(EntityAliasVisitor&& rhs) -> EntityAliasVisitor&
    {
        if (this != &rhs)
        {
            this->~EntityAliasVisitor();
            *this = EntityAliasVisitor(rhs.m_owner, rhs.m_entityAliasList);
            m_dirty = rhs.m_dirty;

            rhs.m_entityAliasList = nullptr;
            rhs.m_dirty = false;
        }
        return *this;
    }

    bool Spawnable::EntityAliasVisitor::HasLock() const
    {
        return EntityAliasVisitorBase::HasLock(m_entityAliasList);
    }

    bool Spawnable::EntityAliasVisitor::HasAliases() const
    {
        return EntityAliasVisitorBase::HasAliases(m_entityAliasList);
    }

    bool Spawnable::EntityAliasVisitor::AreAllSpawnablesReady() const
    {
        return EntityAliasVisitorBase::AreAllSpawnablesReady(m_entityAliasList);
    }

    auto Spawnable::EntityAliasVisitor::begin() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::begin(m_entityAliasList);
    }

    auto Spawnable::EntityAliasVisitor::end() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::end(m_entityAliasList);
    }

    auto Spawnable::EntityAliasVisitor::cbegin() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::cbegin(m_entityAliasList);
    }

    auto Spawnable::EntityAliasVisitor::cend() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::cend(m_entityAliasList);
    }

    void Spawnable::EntityAliasVisitor::ListTargetSpawnables(const ListTargetSpawanblesCallback& callback) const
    {
        EntityAliasVisitorBase::ListTargetSpawnables(m_entityAliasList, callback);
    }

    void Spawnable::EntityAliasVisitor::ListTargetSpawnables(AZ::Crc32 tag, const ListTargetSpawanblesCallback& callback) const
    {
        EntityAliasVisitorBase::ListTargetSpawnables(m_entityAliasList, tag, callback);
    }

    void Spawnable::EntityAliasVisitor::AddAlias(
        AZ::Data::Asset<Spawnable> targetSpawnable,
        AZ::Crc32 tag,
        uint32_t sourceIndex,
        uint32_t targetIndex,
        Spawnable::EntityAliasType aliasType,
        bool queueLoad)
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        AZ_Assert(sourceIndex < m_owner.GetEntities().size(), "Invalid source index (%i) for entity alias", sourceIndex);
        if (targetSpawnable.IsReady())
        {
            AZ_Assert(
                targetIndex < targetSpawnable->GetEntities().size(), "Invalid target index (%i) for entity alias '%s'", targetIndex,
                targetSpawnable.GetHint().c_str());
        }

        m_entityAliasList->push_back(Spawnable::EntityAlias{ targetSpawnable, tag, sourceIndex, targetIndex, aliasType, queueLoad });
        m_dirty = true;
    }

    void Spawnable::EntityAliasVisitor::ListSpawnablesPendingLoad(const ListSpawnablesPendingLoadCallback& callback)
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        for (Spawnable::EntityAlias& alias : *m_entityAliasList)
        {
            if (alias.m_queueLoad &&
                alias.m_aliasType != Spawnable::EntityAliasType::Original &&
                alias.m_aliasType != Spawnable::EntityAliasType::Disabled &&
                !alias.m_spawnable.IsLoading() &&
                !alias.m_spawnable.IsReady() &&
                !alias.m_spawnable.IsError())
            {
                callback(alias.m_spawnable);
            }
        }
    }

    void Spawnable::EntityAliasVisitor::UpdateAliases(const UpdateCallback& callback)
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        for (Spawnable::EntityAlias& alias : *m_entityAliasList)
        {
            AZ::Data::Asset<Spawnable> targetSpawnable(alias.m_spawnable);
            callback(alias.m_aliasType, alias.m_queueLoad, targetSpawnable, alias.m_tag, alias.m_sourceIndex, alias.m_targetIndex);
        }
        m_dirty = true;
    }

    void Spawnable::EntityAliasVisitor::UpdateAliases(AZ::Crc32 tag, const UpdateCallback& callback)
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        for (Spawnable::EntityAlias& alias : *m_entityAliasList)
        {
            if (alias.m_tag == tag)
            {
                AZ::Data::Asset<Spawnable> targetSpawnable(alias.m_spawnable);
                callback(alias.m_aliasType, alias.m_queueLoad, targetSpawnable, alias.m_tag, alias.m_sourceIndex, alias.m_targetIndex);
                m_dirty = true;
            }
        }
    }

    void Spawnable::EntityAliasVisitor::UpdateAliasType(uint32_t index, Spawnable::EntityAliasType newType)
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        AZ_Assert(
            index < m_entityAliasList->size(), "Unable to update entity alias at index %i as there are only %zu aliases in spawnable.",
            index, m_entityAliasList->size());
        (*m_entityAliasList)[index].m_aliasType = newType;
        m_dirty = true;
    }

    void Spawnable::EntityAliasVisitor::Optimize()
    {
        AZ_Assert(m_entityAliasList, "Attempting to visit entity aliases on a spawnable that wasn't locked.");
        if (m_dirty)
        {
            AZStd::stable_sort(
                m_entityAliasList->begin(), m_entityAliasList->end(),
                [](const Spawnable::EntityAlias& lhs, const Spawnable::EntityAlias& rhs)
                {
                    // Sort by source index from smallest to largest so during spawning the entities can be iterated linearly over.
                    // If the source index is the same then sort by alias type so the next steps can optimize away superfluous steps.
                    return lhs.HasLowerIndex(rhs);
                });

            // Remove aliases that are not going to have any practical effect and insert aliases where needed to simplify the spawning.
            // This is done at runtime rather than at build time because the above ebus allows other systems to make adjustments to the
            // aliases, for instance Networking can decide to disable certain aliases when running on a client. This in turn also requires
            // the aliases to be in their recorded order during building as the ebus handlers may depend on that order to determine what
            // entities need to be updated.
            Spawnable::EntityAlias* compare = m_entityAliasList->begin();
            Spawnable::EntityAlias* it = m_entityAliasList->begin() + 1;
            Spawnable::EntityAlias* end = m_entityAliasList->end();
            while (it < end)
            {
                switch (it->m_aliasType)
                {
                case Spawnable::EntityAliasType::Original:
                    // If this is the only alias for the entity then the original can be removed.
                    {
                        Spawnable::EntityAlias* next = it + 1;
                        if (next == end || next->m_sourceIndex != it->m_sourceIndex)
                        {
                            // Erase instead of a swap-and-pop in order to preserver the order.
                            m_entityAliasList->erase(compare);
                            --end;
                            break;
                        }
                    }
                    [[fallthrough]];
                case Spawnable::EntityAliasType::Disabled:
                    [[fallthrough]];
                case Spawnable::EntityAliasType::Replace:
                    // If the previous entry was a disabled, original or replace alias then remove it as it will be overwritten by the
                    // current entry.
                    if (compare->m_sourceIndex == it->m_sourceIndex &&
                        (compare->m_aliasType == Spawnable::EntityAliasType::Original ||
                         compare->m_aliasType == Spawnable::EntityAliasType::Disabled ||
                         compare->m_aliasType == Spawnable::EntityAliasType::Replace))
                    {
                        // Erase instead of a swap-and-pop in order to preserver the order.
                        m_entityAliasList->erase(compare);
                        --end;
                    }
                    else
                    {
                        ++compare;
                        ++it;
                    }
                    break;
                case Spawnable::EntityAliasType::Additional:
                    [[fallthrough]];
                case Spawnable::EntityAliasType::Merge:
                    // If this is the first entry for this type insert an original in front of it so the spawnable entity manager
                    // does have to check for the case there's a merge and/or addition without a prefix.
                    if (compare->m_sourceIndex != it->m_sourceIndex)
                    {
                        Spawnable::EntityAlias insert;
                        // No load, as the asset is already loaded.
                        insert.m_spawnable = AZ::Data::Asset<Spawnable>(&m_owner, AZ::Data::AssetLoadBehavior::NoLoad);
                        insert.m_sourceIndex = it->m_sourceIndex;
                        insert.m_targetIndex = it->m_sourceIndex; // Source index as the original entry for this slot is added.
                        insert.m_aliasType = Spawnable::EntityAliasType::Original;
                        m_entityAliasList->insert(compare, AZStd::move(insert));
                        compare += 2;
                        it += 2;
                        ++end;
                    }
                    else
                    {
                        ++compare;
                        ++it;
                    }
                    break;
                default:
                    AZ_Assert(false, "Invalid Spawnable entity alias type found during asset loading: %i", compare->m_aliasType);
                    break;
                }
            }
            // Reclaim memory because after this point the aliases will not change anymore.
            m_entityAliasList->shrink_to_fit();
            m_dirty = false;
        }
    }



    //
    // EntityAliasConstVisitor
    //

    Spawnable::EntityAliasConstVisitor::EntityAliasConstVisitor(const Spawnable& owner, const EntityAliasList* entityAliasList)
        : m_owner(owner)
        , m_entityAliasList(entityAliasList)
    {
    }

    Spawnable::EntityAliasConstVisitor::~EntityAliasConstVisitor()
    {
        if (HasLock())
        {
            AZ_Assert(
                m_owner.m_lockState < 0, "Attempting to unlock a read shared spawnable that was not in a read shared mode (%i).",
                m_owner.m_lockState.load());
            m_owner.m_lockState++;
        }
    }

    bool Spawnable::EntityAliasConstVisitor::HasLock() const
    {
        return EntityAliasVisitorBase::HasLock(m_entityAliasList);
    }

    bool Spawnable::EntityAliasConstVisitor::HasAliases() const
    {
        return EntityAliasVisitorBase::HasAliases(m_entityAliasList);
    }

    bool Spawnable::EntityAliasConstVisitor::AreAllSpawnablesReady() const
    {
        return EntityAliasVisitorBase::AreAllSpawnablesReady(m_entityAliasList);
    }

    auto Spawnable::EntityAliasConstVisitor::begin() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::begin(m_entityAliasList);
    }

    auto Spawnable::EntityAliasConstVisitor::end() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::end(m_entityAliasList);
    }

    auto Spawnable::EntityAliasConstVisitor::cbegin() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::cbegin(m_entityAliasList);
    }

    auto Spawnable::EntityAliasConstVisitor::cend() const -> EntityAliasList::const_iterator
    {
        return EntityAliasVisitorBase::cend(m_entityAliasList);
    }

    void Spawnable::EntityAliasConstVisitor::ListTargetSpawnables(const ListTargetSpawanblesCallback& callback) const
    {
        EntityAliasVisitorBase::ListTargetSpawnables(m_entityAliasList, callback);
    }

    void Spawnable::EntityAliasConstVisitor::ListTargetSpawnables(AZ::Crc32 tag, const ListTargetSpawanblesCallback& callback) const
    {
        EntityAliasVisitorBase::ListTargetSpawnables(m_entityAliasList, tag, callback);
    }



    //
    // Spawnable
    //

    Spawnable::Spawnable(const AZ::Data::AssetId& id, AssetStatus status)
        : AZ::Data::AssetData(id, status)
    {
    }

    const Spawnable::EntityList& Spawnable::GetEntities() const
    {
        return m_entities;
    }

    Spawnable::EntityList& Spawnable::GetEntities()
    {
        return m_entities;
    }

    auto Spawnable::TryGetAliasesConst() const -> EntityAliasConstVisitor
    {
        int32_t expected = LockState::Unlocked;
        do
        {
            // Try to set the lock to a negative number to indicate a shared read.
            if (m_lockState.compare_exchange_strong(expected, expected - 1))
            {
                return EntityAliasConstVisitor(*this, &m_entityAliases);
            }
        // as long as the value is negative keep trying to get a shared read lock.
        } while (expected <= 0);
        return EntityAliasConstVisitor(*this, nullptr);
    }

    auto Spawnable::TryGetAliases() const -> EntityAliasConstVisitor
    {
        return TryGetAliasesConst();
    }

    auto Spawnable::TryGetAliases() -> EntityAliasVisitor
    {
        int32_t expected = LockState::Unlocked;
        return m_lockState.compare_exchange_strong(expected, LockState::Locked) ? EntityAliasVisitor(*this, &m_entityAliases)
                                                                                : EntityAliasVisitor(*this, nullptr);
    }

    bool Spawnable::IsEmpty() const
    {
        return m_entities.empty();
    }

    bool Spawnable::IsPermanentlyLocked() const
    {
        return m_lockState == LockState::PermanentLock;
    }

    bool Spawnable::LockPermanently()
    {
        if (!IsPermanentlyLocked())
        {
            int32_t expected = LockState::Unlocked;
            return m_lockState.compare_exchange_strong(expected, LockState::PermanentLock);
        }
        else
        {
            return true;
        }
    }

    SpawnableMetaData& Spawnable::GetMetaData()
    {
        return m_metaData;
    }

    const SpawnableMetaData& Spawnable::GetMetaData() const
    {
        return m_metaData;
    }

    void Spawnable::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context); serializeContext != nullptr)
        {
            serializeContext->Class<Spawnable::EntityAlias>()
                ->Version(1)
                ->Field("Spawnable", &Spawnable::EntityAlias::m_spawnable)
                ->Field("Tag", &Spawnable::EntityAlias::m_tag)
                ->Field("Source Index", &Spawnable::EntityAlias::m_sourceIndex)
                ->Field("Target Index", &Spawnable::EntityAlias::m_targetIndex)
                ->Field("Alias Type", &Spawnable::EntityAlias::m_aliasType)
                ->Field("Queue Load", &Spawnable::EntityAlias::m_queueLoad);

            serializeContext->Class<Spawnable, AZ::Data::AssetData>()->Version(2)
                ->Field("Meta data", &Spawnable::m_metaData)
                ->Field("Entity aliases", &Spawnable::m_entityAliases)
                ->Field("Entities", &Spawnable::m_entities);
        }
    }
} // namespace AzFramework
