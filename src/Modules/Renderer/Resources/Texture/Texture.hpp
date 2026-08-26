#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Modules/Renderer/Resources/Shader/TextureFIlter.hpp"
#include "TextureWrap.hpp"
#include "Utilities/Image/Image.hpp"

namespace N {
/**
 * @brief Represents an OpenGL 2D texture resource.
 *
 * A Texture owns its underlying OpenGL texture object and releases it
 * when destroyed. GPU resources are created lazily when the texture is first bound.
 */
struct Texture : Resource {
    /** @brief Image data used to create and upload the texture. */
    U::Image Image;

    /** @brief Wrapping mode applied to texture coordinates along the S axis. */
    TextureWrap WrapS = TextureWrap::MirroredRepeat;

    /** @brief Wrapping mode applied to texture coordinates along the T axis. */
    TextureWrap WrapT = TextureWrap::MirroredRepeat;

    /** @brief Filtering mode used when the texture is minified. */
    TextureFilter MinFilter = TextureFilter::NearestMipmapLinear;

    /** @brief Filtering mode used when the texture is magnified. */
    TextureFilter MagFilter = TextureFilter::Linear;

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

    /**
     * @brief Gets the OpenGL texture object ID.
     * @return OpenGL texture ID, or 0 if the texture has not been loaded.
     */
    [[nodiscard]] unsigned int GetId() const;

    /**
     * @brief Releases the current OpenGL texture object.
     * The texture can be loaded again afterward with Load().
     */
    void Reload();

    /**
     * @brief Checks whether the texture has been loaded to the GPU.
     *
     * @return true if an OpenGL texture object exists, otherwise false.
     */
    bool IsLoaded() const;

    /**
     * @brief Loads the image data into an OpenGL texture object.
     *
     * Creates the OpenGL texture, configures its parameters, uploads the
     * image data, and generates mipmaps.
     */
    void Load();

    /**
     * @brief Binds the texture to a texture unit.
     *
     * If the texture has not been loaded, it is loaded automatically.
     *
     * @param unit Texture unit to bind to.
     */
    void Bind(unsigned int unit);

private:
    /**
     * @brief Configures the texture's wrapping and filtering parameters.
     *
     * Must be called while the texture is bound to GL_TEXTURE_2D.
     */
    void SetParameters() const;

    /// OpenGL texture object ID. Zero indicates that no texture object exists.
    unsigned int Id = 0;
};
} // namespace N
