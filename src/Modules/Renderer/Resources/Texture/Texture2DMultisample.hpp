#pragma once
#include <string>

#include "Texture.hpp"

namespace N {
/** @brief Represents a multisampled two-dimensional OpenGL texture resource. */
struct Texture2DMultisample : Texture {
    /** Number of samples per texel. */
    int Samples = 4;

    /** @brief Creates a 2D multisample texture resource. */
    Texture2DMultisample(const std::string& name) : Texture(name, TextureTarget::Texture2DMultisample) {
    }

    /** @brief Generates the OpenGL multisample texture object. */
    void Generate() override;
};
} // namespace N
