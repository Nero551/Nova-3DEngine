#pragma once
#include "OpenGL.hpp"

namespace N {
enum class StencilAction : GLenum {
    Keep = GL_KEEP,
    Zero = GL_ZERO,
    Replace = GL_REPLACE,
    Increment = GL_INCR,
    IncrementWrap = GL_INCR_WRAP,
    Decrement = GL_DECR,
    DecrementWrap = GL_DECR_WRAP,
    Invert = GL_INVERT
};
}
