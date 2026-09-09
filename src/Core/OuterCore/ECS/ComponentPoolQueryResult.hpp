#pragma once
#include "ComponentPool.hpp"

namespace N
{

struct IComponentPoolQueryResult
{
    virtual ~IComponentPoolQueryResult() = default;
};

template <ComponentType... Args> struct ComponentPoolQueryResult : IComponentPoolQueryResult
{
    std::vector<unsigned int> EntityIds{};
    std::tuple<std::vector<Args*>...> Components;

    struct Iterator
    {
        ComponentPoolQueryResult& Result;
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
} // namespace N
