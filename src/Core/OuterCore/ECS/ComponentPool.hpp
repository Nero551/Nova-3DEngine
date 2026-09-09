#pragma once
#include "Component.hpp"

#include "Core/Services/EventBus.hpp"
#include "Utilities/SparseSet.hpp"
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
            return {Pool.Components.Indices[Index], Pool.Components.Dense[Index]};
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
        return {.Pool = *this, .Index = 0};
    }

    Iterator end()
    {
        return {.Pool = *this, .Index = Components.Size()};
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

    T& Add(unsigned int entityId)
    {
        return Components.Emplace(entityId);
    }

    bool HasId(unsigned int entityId) const
    {
        return Components.Contains(entityId);
    }

    T& GetComponentById(unsigned int entityId)
    {
        return Components.Get(entityId);
    }

    unsigned int GetIdByIndex(size_t index) const
    {
        return Components.Indices[index];
    }

    T& GetComponentByIndex(size_t index)
    {
        return Components.Dense[index];
    }

    void RemoveById(unsigned int entityId)
    {
        Components.Delete(entityId);
    }

    size_t Size() const
    {
        return Components.Size();
    }

  private:
    SparseSet<T> Components{};
};
} // namespace N