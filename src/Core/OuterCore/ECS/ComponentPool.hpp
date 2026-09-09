#pragma once

#include "Component.hpp"

#include "Core/Services/EventBus.hpp"
#include "Utilities/DataStructures/SparseSet.hpp"
#include "World/Events/EntityDestroyed.hpp"

namespace N
{

/**
 * @brief Type-erased base class for component pools.
 */
struct IComponentPool
{
    virtual ~IComponentPool() = default;
};

/** @brief Defines a valid component type. */
template <typename T>
concept ComponentType = std::derived_from<T, Component>;

/**
 * @brief Stores components using a sparse set.
 *
 * Provides O(1) component lookup by entity ID while keeping components
 * densely packed for efficient iteration.
 *
 * Automatically removes components when their associated entity is destroyed.
 *
 * @tparam T Component type stored by the pool.
 */
template <ComponentType T> struct ComponentPool : IComponentPool
{
    /**
     * @brief Iterator over the components in dense storage.
     *
     * Dereferencing the iterator returns the entity ID and corresponding component.
     */
    struct Iterator
    {
        ComponentPool& Pool;
        size_t Index;

        /** @brief Returns the entity ID and corresponding component. */
        std::pair<unsigned int, T&> operator*() const
        {
            return {Pool.Components.GetSparseIndex(Index), Pool.Components.GetByIndex(Index)};
        }

        /** @brief Advances the iterator to the next component. */
        Iterator& operator++()
        {
            ++Index;
            return *this;
        }

        /** @brief Compares two iterators for inequality. */
        bool operator!=(const Iterator& other) const
        {
            return Index != other.Index;
        }
    };

    /** @brief Returns an iterator to the first component. */
    Iterator begin()
    {
        return {.Pool = *this, .Index = 0};
    }

    /** @brief Returns an iterator past the last component. */
    Iterator end()
    {
        return {.Pool = *this, .Index = Components.Size()};
    }

    /**
     * @brief Constructs and adds a component for an entity.
     *
     * @param entityId ID of the entity receiving the component.
     * @return Reference to the stored component.
     */
    T& Add(const unsigned int entityId)
    {
        return Components.Emplace(entityId);
    }

    /** @brief Returns whether the specified entity has this component. */
    bool HasId(const unsigned int entityId) const
    {
        return Components.Contains(entityId);
    }

    /**
     * @brief Returns the component belonging to an entity.
     *
     * @param entityId ID of the entity.
     * @return Reference to the stored component.
     */
    T& GetComponentById(const unsigned int entityId)
    {
        return Components.Get(entityId);
    }

    /**
     * @brief Returns the entity ID at a dense storage index.
     *
     * @param index Dense index of the component.
     * @return Entity ID associated with the component.
     */
    unsigned int GetIdByIndex(const size_t index) const
    {
        return Components.GetSparseIndex(index);
    }

    /**
     * @brief Returns the component at a dense storage index.
     *
     * @param index Dense index of the component.
     * @return Reference to the stored component.
     */
    T& GetComponentByIndex(const size_t index)
    {
        return Components.GetByIndex(index);
    }

    /**
     * @brief Removes the component belonging to an entity.
     *
     * @param entityId ID of the entity whose component should be removed.
     */
    void RemoveById(const unsigned int entityId)
    {
        Components.Delete(entityId);
    }

    /** @brief Returns the number of stored components. */
    size_t Size() const
    {
        return Components.Size();
    }

  private:
    /** @brief Stores components using dense storage with sparse entity ID lookup. */
    SparseSet<T> Components{};
};

} // namespace N