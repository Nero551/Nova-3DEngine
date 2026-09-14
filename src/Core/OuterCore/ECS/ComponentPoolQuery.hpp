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
 * a callback with references to their components. Matching entity IDs are
 * cached and reused until the query structure changes.
 */
struct ComponentPoolQuery
{
    /**
     * @brief Returns the component pool for a type, creating it if needed.
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

    /** @brief Stores the cached results and version of a component query. */
    struct QueryCache
    {
        std::vector<unsigned int> Entities;
        unsigned int Version = 0;
    };

    /**
     * @brief Iterates over entities containing all specified components.
     *
     * The first component acts as the driver pool. Matching entity IDs are
     * cached so subsequent calls avoid rebuilding the query until the query
     * structure changes.
     *
     * @tparam First First component type and driver pool.
     * @tparam Rest Additional component types.
     * @tparam Function Callback type.
     *
     * @param callback Function invoked with the entity ID and component references.
     */
    template <ComponentType First, ComponentType... Rest, typename Function>
    requires std::invocable<Function, unsigned int, First&, Rest&...>
    void ForEach(Function&& callback)
    {
        auto pools = GetPools<First, Rest...>();

        if (!CachedQueries.Contains<First, Rest...>())
        {
            CachedQueries.Emplace<First, Rest...>();
        }

        QueryCache& cache = CachedQueries.Get<First, Rest...>();

        if (cache.Version == QueryVersion)
        {
            ComponentPool<First>& firstPool = std::get<ComponentPool<First>&>(pools);

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
    /** @brief Version used to detect changes that can invalidate query caches. */
    unsigned int QueryVersion = 1;

    /** @brief Stores all component pools indexed by their component type. */
    TypedVector<std::unique_ptr<IComponentPool>> ComponentPools{};

    /** @brief Stores cached results for each component query. */
    TypedVector<QueryCache> CachedQueries;

    /**
     * @brief Returns the component pools for the specified types.
     *
     * @tparam Args Component types to retrieve.
     * @return Tuple containing references to the requested component pools.
     */
    template <ComponentType... Args> std::tuple<ComponentPool<Args>&...> GetPools()
    {
        return {Pool<Args>()...};
    }

    /**
     * @brief Subscribes to events that can change query membership.
     *
     * Each relevant event increments the query version, causing cached
     * entity lists to be rebuilt on their next use.
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