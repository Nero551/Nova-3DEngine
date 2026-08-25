#pragma once
#include "OpenGL.hpp"

namespace N {
enum class ShaderStage {
    Fragment = GL_FRAGMENT_SHADER,
    Vertex = GL_VERTEX_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
};
} // namespace N
