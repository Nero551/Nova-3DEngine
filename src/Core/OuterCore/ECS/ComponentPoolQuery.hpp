#pragma once

#include "ComponentPool.hpp"
#include "World/Events/EntityCreated.hpp"

namespace N
{

/**
 * @brief Provides cached queries over component pools.
 *
 * Queries match entities containing all requested component types and return
 * references to their components through reusable query results.
 *
 * Query results are automatically invalidated when the world's entity or
 * component structure changes and are rebuilt on the next query.
 */
struct ComponentPoolQuery
{
    /**
     * @brief Returns the component pool for the specified type.
     *
     * Creates the pool if it does not already exist.
     *
     * @tparam T Component type.
     * @return Reference to the component pool.
     */
    template <ComponentType T> ComponentPool<T>& Pool()
    {
        const TypeId typeId = GetTypeId<T>();

        if (typeId >= ComponentPools.size())
            ComponentPools.resize(typeId + 1);

        if (!ComponentPools[typeId])
            ComponentPools[typeId] = std::make_unique<ComponentPool<T>>();

        return static_cast<ComponentPool<T>&>(*ComponentPools[typeId]);
    }

    template <ComponentType First, ComponentType... Rest, typename Function>
    requires std::invocable<Function, unsigned int, First&, Rest&...>
    void ForEach(Function&& callback)
    {
        auto& firstPool = Pool<First>();

        for (size_t i = 0; i < firstPool.Size(); ++i)
        {
            const unsigned int entityId = firstPool.GetIdByIndex(i);

            if (!(Pool<Rest>().HasId(entityId) && ...))
            {
                continue;
            }

            callback(entityId, firstPool.GetComponentByIdUnChecked(entityId),
                Pool<Rest>().GetComponentByIdUnChecked(entityId)...);
        }
    }

  private:
    /**
     * @brief Version of the current entity/component structure.
     *
     * Incremented whenever an event occurs that can change query membership.
     * Cached query results compare their stored version against this value
     * to determine whether they must be rebuilt.
     */
    unsigned int QueryVersion = 1;

    /**
     * @brief Stores heterogeneous component pools by component type.
     *
     * Component types are identified using their std::type_index.
     *
     */
    std::vector<std::unique_ptr<IComponentPool>> ComponentPools{};

    using TypeId = size_t;
    inline static size_t NextTypeId{};
    template <typename T> TypeId GetTypeId()
    {
        static const TypeId Id = NextTypeId++;
        return Id;
    }

    /**
     * @brief Returns the smallest component pool in a query.
     *
     * The smallest pool is used as the query's driver pool, reducing the
     * number of entity membership checks required when rebuilding a result.
     *
     * @tparam Args Component types contained in the query.
     * @param pools Component pools to compare.
     * @return Reference to the smallest component pool.
     */
    template <ComponentType... Args>
    IComponentPool& GetSmallestPool(std::tuple<ComponentPool<Args>&...>& pools)
    {
        IComponentPool* smallest = &std::get<0>(pools);

        std::apply([&smallest](auto&... pool)
            { (..., (smallest = pool.Size() < smallest->Size() ? &pool : smallest)); }, pools);

        return *smallest;
    }

    /**
     * @brief Returns the component pools for the specified component types.
     *
     * @tparam Args Component types to retrieve.
     * @return Tuple containing references to the requested component pools.
     */
    template <ComponentType... Args> std::tuple<ComponentPool<Args>&...> GetPools()
    {
        return {Pool<Args>()...};
    }

    /**
     * @brief Subscribes to events that can invalidate cached queries.
     *
     * Query results are invalidated when entities are created or destroyed,
     * or when a component is added to an entity.
     */
    void SubscribeToEvents()
    {
        Service::Get<EventBus>().Sub<EntityDestroyed>([this](const EntityDestroyed&) { ++QueryVersion; });

        Service::Get<EventBus>().Sub<EntityCreated>([this](const EntityCreated&) { ++QueryVersion; });

        Service::Get<EventBus>().Sub<ComponentAdded>([this](const ComponentAdded&) { ++QueryVersion; });
    }

    friend struct World;
};

} // namespace N