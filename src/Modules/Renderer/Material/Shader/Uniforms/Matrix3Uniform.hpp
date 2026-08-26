#pragma once
#include "Math/Matrix/Matrix4.hpp"
#include "OpenGL.hpp"
#include "Uniform.hpp"

namespace N {
struct Matrix3Uniform : Uniform {
    M::Matrix3 Value;

    Matrix3Uniform(const std::string& name, const M::Matrix3& value) : Uniform(name), Value(value) {
    }

    void Upload(const int location) override {
        glUniformMatrix3fv(location, 1, GL_TRUE, *Value.m);
    }
};
} // namespace N
