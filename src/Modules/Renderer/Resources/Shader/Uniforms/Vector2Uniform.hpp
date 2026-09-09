#pragma once
#include "Math/Vector/Vector2.hpp"
#include "Uniform.hpp"

namespace N
{
struct Vector2Uniform : Uniform
{
    N::M::Vector2 Value;

    Vector2Uniform(const std::string_view name, const N::M::Vector2& value)
        : Uniform(name), Value(value)
    {
    }

    void Upload(int location) override
    {
        glUniform2fv(location, 1, &Value.x);
    }
};
} // namespace N
