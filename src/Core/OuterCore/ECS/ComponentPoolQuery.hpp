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
     * @brief Type-erased base class for cached query results.
     *
     * Allows query results of different component types to be stored together
     * in a single collection.
     */
    struct IQueryResult
    {
        virtual ~IQueryResult() = default;
    };

    /**
     * @brief Stores the cached results of a component query.
     *
     * Each result contains the IDs of matching entities and pointers to their
     * requested components. Component pointers are stored to avoid repeatedly
     * looking up components while iterating over a query.
     *
     * @tparam Args Component types requested by the query.
     */
    template <ComponentType... Args> struct QueryResult : IQueryResult
    {
        /** @brief Entity IDs matching the query. */
        std::vector<unsigned int> EntityIds{};

        /** @brief Component pointers corresponding to each matching entity. */
        std::tuple<std::vector<Args*>...> Components{};

        /**
         * @brief Version of the query structure used to build this result.
         *
         * The result is rebuilt when this version differs from the owning
         * ComponentPoolQuery's current version.
         */
        unsigned Version = 0;

        /**
         * @brief Iterates over the entities and their requested components.
         */
        struct Iterator
        {
            QueryResult& Result;
            size_t Index;

            /**
             * @brief Returns an entity ID and references to its components.
             *
             * @return Tuple containing the entity ID followed by references
             * to all requested components.
             */
            std::tuple<unsigned int, Args&...> operator*() const
            {
                return {Result.EntityIds[Index], *std::get<std::vector<Args*>>(Result.Components)[Index]...};
            }

            /** @brief Advances the iterator to the next result. */
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

        /** @brief Returns an iterator to the first query result. */
        Iterator begin()
        {
            return {*this, 0};
        }

        /** @brief Returns an iterator past the last query result. */
        Iterator end()
        {
            return {*this, EntityIds.size()};
        }

        /**
         * @brief Removes all query results while retaining allocated capacity.
         *
         * Component pointer storage and entity ID storage are cleared but
         * their allocated memory is retained for subsequent query rebuilds.
         */
        void Clear()
        {
            EntityIds.clear();

            std::apply([](auto&... components) { (components.clear(), ...); }, Components);
        }

        /**
         * @brief Reserves storage for the specified number of results.
         *
         * Reserves capacity for entity IDs and every component pointer array.
         *
         * @param size Number of query results to reserve.
         */
        void Reserve(const size_t size)
        {
            EntityIds.reserve(size);

            std::apply([size](auto&... components) { (components.reserve(size), ...); }, Components);
        }
    };

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

    /**
     * @brief Queries for entities containing all specified component types.
     *
     * Results are cached and reused between calls. A query is only rebuilt
     * when the component pool structure or entity collection has changed.
     *
     * The smallest component pool is used as the driver pool during a rebuild
     * to minimize the number of entities that must be checked.
     *
     * @tparam Args Component types required by the query.
     * @return Reference to the cached query result.
     */
    template <ComponentType... Args> QueryResult<Args...>& With()
    {
        QueryResult<Args...>& result = GetQueryResult<Args...>();

        //TODO- a more efficient method is giving each component pool a version.
        // and just comparing the result's version with the component pools it needs.
        if (result.Version != QueryVersion)
        {
            std::tuple<ComponentPool<Args>&...> pools = GetPools<Args...>();
            auto& pool = std::get<0>(pools);

            result.Clear();
            PopulateResult(result, pools);
            result.Version = QueryVersion;
        }

        return result;
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
     * @brief Stores heterogeneous cached query results by query type.
     *
     * Each unique combination of requested component types has its own cached
     * QueryResult instance.
     */
    std::unordered_map<std::type_index, std::unique_ptr<IQueryResult>> QueryResults{};

    /**
     * @brief Returns the cached query result for the specified component types.
     *
     * Creates the result if it does not already exist.
     *
     * @tparam Args Component types contained in the query.
     * @return Reference to the cached query result.
     */
    template <ComponentType... Args> QueryResult<Args...>& GetQueryResult()
    {
        const std::type_index key = typeid(QueryResult<Args...>);

        auto it = QueryResults.find(key);

        if (it == QueryResults.end())
        {
            it = QueryResults.emplace(key, std::make_unique<QueryResult<Args...>>()).first;
        }

        return static_cast<QueryResult<Args...>&>(*it->second);
    }

    /**
     * @brief Populates a query result with matching entities.
     *
     * Iterates over the smallest component pool and checks whether each
     * entity also exists in every other requested pool.
     *
     * @tparam Args Component types contained in the query.
     * @param result Query result to populate.
     * @param pools Component pools used by the query.
     */
    template <ComponentType... Args>
    void PopulateResult(QueryResult<Args...>& result, std::tuple<ComponentPool<Args>&...>& pools)
    {
        auto& driverPool = GetSmallestPool(pools);
        result.Reserve(driverPool.Size());

        for (size_t i = 0; i < driverPool.Size(); ++i)
        {
            const unsigned int id = driverPool.GetIdByIndex(i);

            if ((std::get<ComponentPool<Args>&>(pools).HasId(id) && ...))
            {
                result.EntityIds.push_back(id);
                AddComponents(result, id, pools);
            }
        }
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
     * @brief Adds component pointers for an entity to a query result.
     *
     * Components are stored as pointers so that subsequent query iteration
     * can access them directly without performing another pool lookup.
     *
     * @tparam Args Component types contained in the query.
     * @param result Query result to populate.
     * @param id Entity ID to add.
     * @param Pools Component pools used by the query.
     */
    template <ComponentType... Args>
    void AddComponents(
        QueryResult<Args...>& result, const unsigned int id, std::tuple<ComponentPool<Args>&...>& Pools)
    {
        std::apply(
            [&](auto&... pools)
            {
                (std::get<std::vector<Args*>>(result.Components)
                        .emplace_back(&pools.GetComponentByIdUnChecked(id)),
                    ...);
            },
            Pools);
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