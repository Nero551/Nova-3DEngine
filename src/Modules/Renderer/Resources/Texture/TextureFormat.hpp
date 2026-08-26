#pragma once

#include "OpenGL.hpp"

namespace N {
/**
 * @brief Format of pixel data supplied to OpenGL.
 *
 * Determines which components are present in the source pixel data.
 */
enum class TextureFormat {
    /** Red channel only. */
    Red = GL_RED,

    /** Red and green channels. */
    RG = GL_RG,

    /** Red, green, and blue channels. */
    RGB = GL_RGB,

    /** Blue, green, and red channels. */
    BGR = GL_BGR,

    /** Red, green, blue, and alpha channels. */
    RGBA = GL_RGBA,

    /** Blue, green, red, and alpha channels. */
    BGRA = GL_BGRA,

    /** Depth component only. */
    Depth = GL_DEPTH_COMPONENT,

    /** Depth and stencil components. */
    DepthStencil = GL_DEPTH_STENCIL
};
} // namespace N
