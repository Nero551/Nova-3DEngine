#pragma once

#include "OpenGL.hpp"

namespace N {
/**
 * @brief Internal format used to store texture data on the GPU.
 *
 * Determines the component layout, precision, and storage type
 * used by the GPU for each texel.
 */
enum class TextureInternalFormat {
    /** 8-bit normalized red channel. */
    R8 = GL_R8,

    /** 8-bit normalized red and green channels. */
    RG8 = GL_RG8,

    /** 8-bit normalized RGB channels. */
    RGB8 = GL_RGB8,

    /** 8-bit normalized RGBA channels. */
    RGBA8 = GL_RGBA8,

    /** 16-bit floating-point red channel. */
    R16F = GL_R16F,

    /** 16-bit floating-point red and green channels. */
    RG16F = GL_RG16F,

    /** 16-bit floating-point RGB channels. */
    RGB16F = GL_RGB16F,

    /** 16-bit floating-point RGBA channels. */
    RGBA16F = GL_RGBA16F,

    /** 32-bit floating-point red channel. */
    R32F = GL_R32F,

    /** 32-bit floating-point red and green channels. */
    RG32F = GL_RG32F,

    /** 32-bit floating-point RGB channels. */
    RGB32F = GL_RGB32F,

    /** 32-bit floating-point RGBA channels. */
    RGBA32F = GL_RGBA32F,

    /** 24-bit depth with 8-bit stencil. */
    Depth24Stencil8 = GL_DEPTH24_STENCIL8,

    /** 32-bit floating-point depth. */
    Depth32F = GL_DEPTH_COMPONENT32F,

    /** 32-bit floating-point depth with 8-bit stencil. */
    Depth32FStencil8 = GL_DEPTH32F_STENCIL8,

    /** 8-bit sRGB RGB channels. */
    SRGB8 = GL_SRGB8,

    /** 8-bit sRGB RGB channels with alpha. */
    SRGBA8 = GL_SRGB8_ALPHA8
};
} // namespace N
