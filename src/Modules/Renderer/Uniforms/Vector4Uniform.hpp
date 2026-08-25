#pragma once
#include "../Uniform.hpp"
#include "Math/Vector/Vector4.hpp"

namespace N {
struct Vector4Uniform : Uniform {
    M::Vector4 Value;

    Vector4Uniform(const std::string& name, const M::Vector4& value) : Uniform(name), Value(value) {
    }

    void Upload(const int location) override {
        glUniform4fv(location, 1, &Value.x);
    }
};
} // namespace N
