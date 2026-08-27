#pragma once
#include <string>

#include "Texture.hpp"
#include "Utilities/Image/Image.hpp"

namespace N {
/**
 * @brief Represents an OpenGL cubemap texture resource.
 *
 * A cubemap consists of six 2D image faces representing the positive and
 * negative directions of the X, Y, and Z axes. The cubemap is stored as a
 * single OpenGL texture object and can be sampled using a three-dimensional
 * direction vector.
 */
struct Cubemap : Texture {
    /** Image representing the negative Z face of the cubemap. */
    U::Image Front{};

    /** Image representing the positive Z face of the cubemap. */
    U::Image Back{};

    /** Image representing the positive Y face of the cubemap. */
    U::Image Top{};

    /** Image representing the negative Y face of the cubemap. */
    U::Image Bottom{};

    /** Image representing the positive X face of the cubemap. */
    U::Image Right{};

    /** Image representing the negative X face of the cubemap. */
    U::Image Left{};

    /** Wrapping mode applied to texture coordinates along the R axis. */
    TextureWrap WrapR = TextureWrap::ClampToEdge;

    /**
     * @brief Creates a cubemap texture resource.
     * @param name Resource name.
     */
    Cubemap(const std::string& name) : Texture(name, TextureTarget::Cubemap) {
    }

    /**
     * @brief Generates the OpenGL cubemap texture.
     *
     * Uploads all six image faces to the GPU, configures the cubemap's
     * texture parameters, and optionally generates mipmaps.
     *
     * All six faces must have identical dimensions.
     */
    void Generate() override;

protected:
    /**
     * @brief Configures the cubemap's wrapping and filtering parameters.
     *
     * Applies the base texture parameters and additionally configures
     * wrapping along the R axis.
     */
    void SetParameters() const override;
};
} // namespace N
