#pragma once

#include "ComponentPool.hpp"
#include "Utilities/DataStructures/TypedVector.hpp"
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
        if (!ComponentPools.Contains<T>())
        {
            ComponentPools.Emplace<T>(std::make_unique<ComponentPool<T>>());
        }

        return static_cast<ComponentPool<T>&>(*ComponentPools.Get<T>());
    }

    //TODO- could cache entity list and not do HasId every time, since that's the most expensive.
    // same way i did for query results by subbing to events and updating QueryVersion then.
    // i could give each pool a version, if version doesn't match the cache, rebuild.

    //TODO- i could give the version to the entity list itself, and make pools update their own version on component added.
    // then i do the check here.

    //TODO- have a GetQueryId method just like GetTypeId, but accepts ...Args, will give different ids for every combo of pools.
    // could make a custom container for this type of vector.
    // one that uses this Id thing made from type templates. call it like, TypedVector.

    //TODO- unique pointers aren't cache local, make a new data structure for this.
    // a container for handling objects of different types but same base. (ex: system, module, entity, etc)
    // without using unique pointers.

    struct EntityListCache
    {
        std::vector<unsigned int> Entities;
        unsigned int Version = 0;
    };

    template <ComponentType First, ComponentType... Rest, typename Function>
    requires std::invocable<Function, unsigned int, First&, Rest&...>
    void ForEach(Function&& callback)
    {
        auto pools = GetPools<First, Rest...>();

        if (!QueryCache.Contains<First, Rest...>())
        {
            QueryCache.Emplace<First, Rest...>();
        }

        auto& cache = QueryCache.Get<First, Rest...>();

        if (cache.Version == QueryVersion)
        {
            for (unsigned int entityId : cache.Entities)
            {
                //TODO- GetComponentByIdUnchecked is the bottleneck.
                callback(entityId, std::get<ComponentPool<First>&>(pools).GetComponentByIdUnchecked(entityId),
                    std::get<ComponentPool<Rest>&>(pools).GetComponentByIdUnchecked(entityId)...);
            }

            return;
        }

        cache.Entities.clear();

        auto& firstPool = std::get<ComponentPool<First>&>(pools);

        for (auto [entityId, firstComponent] : firstPool)
        {
            if (!((std::get<ComponentPool<Rest>&>(pools).HasId(entityId)) && ...))
            {
                continue;
            }

            cache.Entities.emplace_back(entityId);

            callback(entityId, firstComponent,
                std::get<ComponentPool<Rest>&>(pools).GetComponentByIdUnchecked(entityId)...);
        }

        cache.Version = QueryVersion;
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
    TypedVector<std::unique_ptr<IComponentPool>> ComponentPools{};
    TypedVector<EntityListCache> QueryCache;

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