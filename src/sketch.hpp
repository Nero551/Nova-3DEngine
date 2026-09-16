#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Utilities/Log.hpp"

#include <concepts>
namespace N
{

template <typename T> struct Traits
{
    static constexpr bool IsComponent = std::derived_from<T, Component>;
};

struct Block : Component
{
};

struct NotBlock
{
};

inline void Test()
{
    U::Log::Info(Traits<Block>::IsComponent);    // true
    U::Log::Info(Traits<NotBlock>::IsComponent); // false
}

} // namespace N