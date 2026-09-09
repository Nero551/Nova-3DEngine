#pragma once
#include "OpenGL.hpp"

namespace N
{
struct Uniform
{
    std::string_view Name;

    Uniform(const std::string_view name) : Name(name) {};
    virtual ~Uniform() {}

    virtual void Upload(int location) {};
};
} // namespace N
