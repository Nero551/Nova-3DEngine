#pragma once
#include "Math/Matrix/Matrix4.hpp"
#include "OpenGL.hpp"
#include "Uniform.hpp"

namespace N {
struct Matrix4Uniform : Uniform {
    M::Matrix4 Value;

    Matrix4Uniform(const std::string& name, const M::Matrix4& value) : Uniform(name), Value(value) {
    }

    void Upload(const int location) override {
        glUniformMatrix4fv(location, 1, GL_TRUE, *Value.m);
    }
};
} // namespace N
