#pragma once

#include "ComponentPool.hpp"

namespace N
{

/**
 * @brief Provides cached queries over component pools.
 *
 * Queries match entities containing all requested component types and return
 * references to their components through a reusable query result.
 */
struct ComponentPoolQuery
{
    /**
     * @brief Type-erased base class for cached query results.
     */
    struct IQueryResult
    {
        virtual ~IQueryResult() = default;
    };

    /**
     * @brief Stores the results of a component query.
     *
     * @tparam Args Component types requested by the query.
     */
    template <ComponentType... Args> struct QueryResult : IQueryResult
    {
        /** @brief Entity IDs matching the query. */
        std::vector<unsigned int> EntityIds{};

        /** @brief Component pointers corresponding to each entity. */
        std::tuple<std::vector<Args*>...> Components{};

        struct Iterator
        {
            QueryResult& Result;
            size_t Index;

            /** @brief Returns the entity ID and corresponding components. */
            std::tuple<unsigned int, Args&...> operator*() const
            {
                return std::apply(
                    [&](auto&... components)
                    {
                        return std::tuple<unsigned int, Args&...>{Result.EntityIds[Index],
                            *std::get<std::vector<Args*>>(Result.Components)[Index]...};
                    },
                    Result.Components);
            }

            /** @brief Advances the iterator. */
            Iterator& operator++()
            {
                ++Index;
                return *this;
            }

            /** @brief Compares two iterators. */
            bool operator!=(const Iterator& other) const
            {
                return Index != other.Index;
            }
        };

        /** @brief Returns an iterator to the first result. */
        Iterator begin()
        {
            return {*this, 0};
        }

        /** @brief Returns an iterator past the last result. */
        Iterator end()
        {
            return {*this, EntityIds.size()};
        }

        /** @brief Removes all results while retaining allocated capacity. */
        void Clear()
        {
            EntityIds.clear();

            std::apply([](auto&... components) { (components.clear(), ...); }, Components);
        }

        /** @brief Reserves storage for the specified number of results. */
        void Reserve(const size_t size)
        {
            EntityIds.reserve(size);

            std::apply(
                [size](auto&... components) { (components.reserve(size), ...); }, Components);
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
        const auto type = std::type_index(typeid(T));
        auto it = ComponentPools.find(type);

        if (it == ComponentPools.end())
        {
            it = ComponentPools.emplace(type, std::make_unique<ComponentPool<T>>()).first;
        }

        return static_cast<ComponentPool<T>&>(*it->second);
    }

    /**
     * @brief Queries for entities containing all specified component types.
     *
     * Results are cached and reused between calls to avoid repeated allocations.
     *
     * @tparam Args Component types required by the query.
     * @return Reference to the cached query result.
     */
    template <ComponentType... Args> QueryResult<Args...>& With()
    {
        std::tuple<ComponentPool<Args>&...> pools = GetPools<Args...>();
        auto& pool = std::get<0>(pools);
        QueryResult<Args...>& result = GetQueryResult<Args...>();

        result.Clear();
        result.Reserve(pool.Size());
        PopulateResult(result, pools);

        return result;
    }

  private:
    /** @brief Stores heterogeneous component pools by component type. */
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> ComponentPools{};

    /** @brief Stores heterogeneous cached query results by query type. */
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
     * @tparam Args Component types contained in the query.
     * @param result Query result to populate.
     * @param pools Component pools used by the query.
     */
    template <ComponentType... Args>
    void PopulateResult(QueryResult<Args...>& result, std::tuple<ComponentPool<Args>&...>& pools)
    {
        auto& firstPool = std::get<0>(pools);

        for (size_t i = 0; i < firstPool.Size(); ++i)
        {
            const unsigned int id = firstPool.GetIdByIndex(i);

            if ((std::get<ComponentPool<Args>&>(pools).HasId(id) && ...))
            {
                result.EntityIds.push_back(id);
                AddComponents(result, id, pools);
            }
        }
    }

    /**
     * @brief Adds component pointers for an entity to a query result.
     *
     * @tparam Args Component types contained in the query.
     * @param result Query result to populate.
     * @param id Entity ID to add.
     * @param pools Component pools used by the query.
     */
    template <ComponentType... Args>
    void AddComponents(QueryResult<Args...>& result, const unsigned int id,
        std::tuple<ComponentPool<Args>&...>& pools)
    {
        std::apply(
            [&](auto&... pool)
            {
                (std::get<std::vector<Args*>>(result.Components)
                        .push_back(&pool.GetComponentById(id)),
                    ...);
            },
            pools);
    }

    /**
     * @brief Returns the component pools for the specified component types.
     *
     * @tparam Args Component types to retrieve.
     * @return Tuple containing references to the component pools.
     */
    template <ComponentType... Args> std::tuple<ComponentPool<Args>&...> GetPools()
    {
        return {Pool<Args>()...};
    }
};

} // namespace N