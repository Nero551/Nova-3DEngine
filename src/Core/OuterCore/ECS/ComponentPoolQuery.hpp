#pragma once
#include "ComponentPool.hpp"

namespace N
{
struct ComponentPoolQuery
{
    //TODO- clean componentPool, Query & QueryResult up , this shit is unreadable. also DOCUMENT.

    struct IQueryResult
    {
        virtual ~IQueryResult() = default;
    };

    template <ComponentType... Args> struct QueryResult : IQueryResult
    {
        std::vector<unsigned int> EntityIds{};
        std::tuple<std::vector<Args*>...> Components;

        struct Iterator
        {
            QueryResult& Result;
            size_t Index;

            std::tuple<unsigned int, Args&...> operator*() const
            {
                return std::apply(
                    [&](std::vector<Args*>&... components)
                    {
                        return std::tuple<unsigned int, Args&...>{
                            Result.EntityIds[Index], *components[Index]...};
                    },
                    Result.Components);
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
            return {.Result = *this, .Index = 0};
        }

        Iterator end()
        {
            return {.Result = *this, .Index = EntityIds.size()};
        }

        /** @brief Removes all query results while retaining allocated capacity. */
        void Clear()
        {
            EntityIds.clear();
            std::apply([](auto&... components) { (components.clear(), ...); }, Components);
        }

        /** @brief Ensures enough storage for the specified number of results. */
        void Reserve(size_t size)
        {
            if (EntityIds.capacity() < size)
                EntityIds.reserve(size);

            std::apply([&](auto&... components)
                { ((components.capacity() < size ? components.reserve(size) : void()), ...); },
                Components);
        }
    };

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
    template <ComponentType... Args> QueryResult<Args...>& With()
    {
        std::tuple<ComponentPool<Args>&...> Pools = GetPools<Args...>();
        auto& firstPool = std::get<0>(Pools);

        const std::type_index key = typeid(QueryResult<Args...>);

        auto it = QueryResults.find(key);

        if (it == QueryResults.end())
        {
            it = QueryResults.emplace(key, std::make_unique<QueryResult<Args...>>()).first;
        }
        else
        {
            static_cast<QueryResult<Args...>&>(*it->second).Clear();
        }

        auto& result = static_cast<QueryResult<Args...>&>(*it->second);
        result.Reserve(firstPool.Size());

        std::apply(
            [&](auto&... pools)
            {
                for (size_t i = 0; i < firstPool.Size(); ++i)
                {
                    const unsigned int id = firstPool.GetIdByIndex(i);

                    if ((pools.HasId(id) && ...))
                    {
                        result.EntityIds.push_back(id);

                        AddComponentsToQueryResult(
                            result, id, Pools, std::index_sequence_for<Args...>{});
                    }
                }
            },
            Pools);

        return result;
    }

  private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> ComponentPools{};
    std::unordered_map<std::type_index, std::unique_ptr<IQueryResult>> QueryResults{};

    template <ComponentType... Args, size_t... I>
    void AddComponentsToQueryResult(QueryResult<Args...>& result, unsigned int id,
        std::tuple<ComponentPool<Args>&...>& pools, const std::index_sequence<I...>&)
    {
        (std::get<I>(result.Components).push_back(&std::get<I>(pools).GetComponentById(id)), ...);
    }

    template <ComponentType... Args> std::tuple<ComponentPool<Args>&...> GetPools()
    {
        return {Pool<Args>()...};
    }
};
} // namespace N
