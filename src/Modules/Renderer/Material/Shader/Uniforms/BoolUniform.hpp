#pragma once
#include "../Uniform.hpp"
#include "OpenGL.hpp"

namespace N {
struct BoolUniform : Uniform {
    bool Value;

    BoolUniform(const std::string& name, bool value) : Uniform(name), Value(value) {
    }

    void Upload(int location) override {
        glUniform1i(location, Value);
    }
};
} // namespace N
