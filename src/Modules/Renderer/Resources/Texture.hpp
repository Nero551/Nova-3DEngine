#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Utilities/Image/Image.hpp"

namespace N {
/**
 * @brief Represents an OpenGL 2D texture resource.
 *
 * A Texture owns its underlying OpenGL texture object and releases it
 * when destroyed. GPU resources are created immediately during construction.
 */
struct Texture : Resource {
    /**
     * @brief Creates a texture using an Image.
     *
     * @param name Resource name.
     * @param image Image to use.
     *
     * @remark The image should be vertically flipped when loaded to account for the difference between image and OpenGL texture
     * coordinates.
     */
    Texture(const std::string& name, const U::Image& image);

    /// Releases the underlying OpenGL texture object.
    ~Texture() override;

    [[nodiscard]] unsigned int GetId() const;

    /**
     * @brief Binds the texture to a texture unit.
     * @param unit Texture unit to bind to.
     */
    void Bind(unsigned int unit) const;

private:
    /// Configures texture wrapping and filtering parameters.
    void SetParameters() const;

    unsigned int Id = 0;
};
} // namespace N
