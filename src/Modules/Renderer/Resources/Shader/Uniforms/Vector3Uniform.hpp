#pragma once
#include "Core/InnerCore/Engine.hpp"
#include "Math/Vector/Vector3.hpp"
#include "Uniform.hpp"

namespace N
{
struct Vector3Uniform : Uniform
{
    M::Vector3 Value;

    Vector3Uniform(const std::string_view name, const M::Vector3& value)
        : Uniform(name), Value(value)
    {
    }

    void Upload(const int location) override
    {
        glUniform3fv(location, 1, &Value.x);
    }
};
} // namespace N
