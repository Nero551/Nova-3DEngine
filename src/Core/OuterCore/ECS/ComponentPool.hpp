#pragma once
#include "Component.hpp"

#include "Core/Services/EventBus.hpp"
#include "World/Events/EntityDestroyed.hpp"

namespace N
{
struct IComponentPool
{
    virtual ~IComponentPool() = default;
};

template <typename T>
concept ComponentType = std::derived_from<T, Component>;

template <ComponentType T> struct ComponentPool : IComponentPool
{
    struct Iterator
    {
        ComponentPool& Pool;
        size_t Index;

        std::pair<unsigned int, T&> operator*() const
        {
            return {Pool.EntityIds[Index], Pool.Components[Index]};
        }

        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }
    };

    Iterator begin()
    {
        return {*this, 0};
    }

    Iterator end()
    {
        return {*this, Components.size()};
    }

    ComponentPool()
    {
        Service::Get<EventBus>().Sub<EntityDestroyed>(
            [this](const EntityDestroyed& event)
            {
                if (HasId(event.entity.Id))
                {
                    RemoveById(event.entity.Id);
                }
            });
    }
    static constexpr size_t InvalidIndex = std::numeric_limits<size_t>::max();

    T& Add(unsigned int entityId)
    {
        if (entityId >= Indices.size())
        {
            Indices.resize(entityId + 1, InvalidIndex);
        }

        const size_t index = Components.size();

        EntityIds.push_back(entityId);
        Components.emplace_back();
        Indices[entityId] = index;

        return Components.back();
    }

    bool HasId(unsigned int entityId) const
    {
        return entityId < Indices.size() && Indices[entityId] != InvalidIndex;
    }

    T& GetComponentById(unsigned int entityId)
    {
        return Components[Indices[entityId]];
    }

    T& GetComponentByIndex(size_t index)
    {
        return Components[index];
    }

    unsigned int GetIdByIndex(size_t index) const
    {
        return EntityIds[index];
    }

    void RemoveById(unsigned int entityId)
    {
        if (!HasId(entityId))
        {
            return;
        }

        RemoveByIndex(Indices[entityId]);
    }

    void RemoveByIndex(size_t index)
    {
        const size_t last = Components.size() - 1;
        const unsigned int removedId = EntityIds[index];

        if (index != last)
        {
            const unsigned int movedId = EntityIds[last];

            Components[index] = std::move(Components[last]);
            EntityIds[index] = movedId;
            Indices[movedId] = index;
        }

        Components.pop_back();
        EntityIds.pop_back();
        Indices[removedId] = InvalidIndex;
    }

    size_t Size() const
    {
        return Components.size();
    }

    //TODO- learn sparse sets and make a sparse set data structure struct

  private:
    std::vector<T> Components;
    std::vector<unsigned int> EntityIds;
    std::vector<size_t> Indices;
};
} // namespace N
