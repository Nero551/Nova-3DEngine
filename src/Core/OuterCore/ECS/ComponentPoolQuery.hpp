#pragma once

#include "ComponentPool.hpp"
#include "Utilities/DataStructures/TypedVector.hpp"
#include "World/Events/EntityCreated.hpp"

namespace N
{

/**
 * @brief Provides cached queries over component pools.
 *
 * Queries match entities containing all requested component types and invoke
 * a callback with references to their components.
 *
 * Matching entity IDs are cached and reused until the world's entity/component
 * structure changes.
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

    /**
     * @brief Caches the entity IDs matching a component query.
     *
     * The cached entity list is rebuilt when its version does not match
     * the current query version.
     */
    struct QueryCache
    {
        std::vector<unsigned int> Entities;
        unsigned int Version = 0;
    };

    /**
     * @brief Iterates over entities containing all specified components.
     *
     * On the first query, the matching entity IDs are determined by iterating
     * over the first component pool and checking the remaining pools.
     * Subsequent calls reuse the cached entity IDs until the query version
     * changes.
     *
     * @tparam First First component type and driver pool.
     * @tparam Rest Additional component types that must be present.
     * @tparam Function Callback type.
     *
     * @param callback Function invoked with the entity ID and references to
     *                 all requested components.
     */
    template <ComponentType First, ComponentType... Rest, typename Function>
    requires std::invocable<Function, unsigned int, First&, Rest&...>
    void ForEach(Function&& callback)
    {
        auto pools = GetPools<First, Rest...>();

        if (!QueryCaches.Contains<First, Rest...>())
        {
            QueryCaches.Emplace<First, Rest...>();
        }

        QueryCache& cache = QueryCaches.Get<First, Rest...>();

        if (cache.Version == QueryVersion)
        {
            auto& firstPool = std::get<ComponentPool<First>&>(pools);

            for (unsigned int entityId : cache.Entities)
            {
                callback(entityId, firstPool.GetComponentByIdUnchecked(entityId),
                    (std::get<ComponentPool<Rest>&>(pools).GetComponentByIdUnchecked(entityId))...);
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

            cache.Entities.push_back(entityId);

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
     * Cached entity lists compare their stored version against this value
     * to determine whether they must be rebuilt.
     */
    unsigned int QueryVersion = 1;

    /**
     * @brief Stores component pools indexed by component type.
     *
     * Each component type has its own pool, while the container provides
     * type-based lookup for heterogeneous pool storage.
     */
    TypedVector<std::unique_ptr<IComponentPool>> ComponentPools{};

    /**
     * @brief Stores cached entity lists indexed by their component query.
     */
    TypedVector<QueryCache> QueryCaches;

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
     * cached entity lists are invalidated when entities are created or destroyed,
     * or when a component is added to an entity.
     */
    void SubscribeToEvents()
    {
        Service::Get<EventBus>().Sub<EntityDestroyed>([this](const EntityDestroyed&) { ++QueryVersion; });

        Service::Get<EventBus>().Sub<EntityCreated>([this](const EntityCreated&) { ++QueryVersion; });

        Service::Get<EventBus>().Sub<ComponentAdded>([this](const ComponentAdded&) { ++QueryVersion; });
        Service::Get<EventBus>().Sub<ComponentRemoved>([this](const ComponentRemoved&) { ++QueryVersion; });
    }

    friend struct World;
};

} // namespace N