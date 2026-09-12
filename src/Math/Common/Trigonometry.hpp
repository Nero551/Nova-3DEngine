#pragma once
#include "Constants.hpp"

namespace N::M
{
constexpr float Rad(const float deg)
{
    return deg * PI / 180;
}

constexpr float Deg(const float rad)
{
    return rad * 180 / PI;
}

constexpr float DSin(const float deg)
{
    return std::sin(Rad(deg));
}

constexpr float DCos(const float deg)
{
    return std::cos(Rad(deg));
}

constexpr float DTan(const float deg)
{
    return std::tan(Rad(deg));
}
} // namespace N::M
