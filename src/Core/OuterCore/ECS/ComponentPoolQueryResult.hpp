#pragma once
#include "ComponentPool.hpp"

namespace N
{
template <ComponentType... Args> struct ComponentPoolQueryResult
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
};
} // namespace N
