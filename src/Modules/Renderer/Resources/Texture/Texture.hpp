#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Modules/Renderer/Resources/Shader/TextureFIlter.hpp"
#include "TextureDataType.hpp"
#include "TextureFormat.hpp"
#include "TextureInternalFormat.hpp"
#include "TextureTarget.hpp"
#include "TextureWrap.hpp"
#include "Utilities/Image/Image.hpp"

namespace N {
/**
 * @brief Represents an OpenGL texture resource.
 *
 * Stores the texture's image data and configuration, and manages the
 * underlying OpenGL texture object. GPU resources are created lazily
 * when Load() or Bind() is called.
 */
struct Texture : Resource {
    // TODO- should texture have an enum for its type? ex: Cubemap , 2D , etc. dont know yet
    //
    // TODO- appears there are other formats other than the color channels, specifically for framebuffer use. gotta figure that
    //  out. could use renderbuffers for those specifically but that limits my API options
    //  easy fix is to just add a custom constructor specifically for those kinds of textures.
    //  could also make the texture not own an image , but have the image transfer and take out all the data into the texture.
    //  that already happens implicitly , since the Image attribute isn't a pointer nor reference.

    /** Data type used to interpret the texture's source pixel data. */
    TextureDataType DataType = TextureDataType::UnsignedByte;

    /** OpenGL texture target defining the texture's dimensionality and layout. */
    TextureTarget Target = TextureTarget::Texture2D;

    /** Format of the source pixel data supplied to OpenGL. */
    TextureFormat Format = TextureFormat::RGBA;

    /** Internal format used to store the texture data on the GPU. */
    TextureInternalFormat InternalFormat = TextureInternalFormat::RGBA8;

    /** Whether mipmaps should be generated automatically when the texture is loaded. */
    bool AutoMipmaps = true;

    /** Width of the texture in pixels. */
    int Width = 0;

    /** Height of the texture in pixels. */
    int Height = 0;

    /** CPU-side pixel data used when loading the texture to the GPU. */
    std::vector<unsigned char> Data;

    /** Wrapping mode applied to texture coordinates along the S axis. */
    TextureWrap WrapS = TextureWrap::MirroredRepeat;

    /** Wrapping mode applied to texture coordinates along the T axis. */
    TextureWrap WrapT = TextureWrap::MirroredRepeat;

    /** Filtering mode used when the texture is minified. */
    TextureFilter MinFilter = TextureFilter::NearestMipmapLinear;

    /** Filtering mode used when the texture is magnified. */
    TextureFilter MagFilter = TextureFilter::Linear;

    /**
     * @brief Creates a texture from an image.
     *
     * Initializes the texture's dimensions, pixel data, format, and default
     * OpenGL configuration from the supplied image.
     *
     * @param name Resource name.
     * @remark The image should be vertically flipped when loaded to account
     * for the difference between image and OpenGL texture coordinates.
     */
    Texture(const std::string& name);

    /** Releases the underlying OpenGL texture object. */
    ~Texture() override;

    /**
     * @brief Replaces the texture's data and configuration.
     *
     * Updates the texture dimensions, pixel data, source format, and internal
     * format using the supplied image. Existing GPU resources are not reloaded.
     *
     * @param image Image to use for the texture.
     */
    void UseImage(const U::Image& image);

    /**
     * @brief Gets the OpenGL texture object ID.
     *
     * @return OpenGL texture ID, or 0 if the texture has not been loaded.
     */
    [[nodiscard]] unsigned int GetId() const;

    /**
     * @brief Releases the current OpenGL texture object.
     *
     * The texture can be loaded again afterward with Load() or Bind().
     */
    void Reload();

    /**
     * @brief Checks whether the texture has been loaded to the GPU.
     *
     * @return true if an OpenGL texture object exists, otherwise false.
     */
    bool IsLoaded() const;

    /**
     * @brief Loads the texture data into an OpenGL texture object.
     *
     * Creates the OpenGL texture, configures its parameters, uploads the
     * stored texture data, and optionally generates mipmaps.
     */
    void Load();

    /**
     * @brief Binds the texture to a texture unit.
     *
     * If the texture has not been loaded, it is loaded automatically.
     *
     * @param unit Texture unit to bind the texture to.
     */
    void Bind(unsigned int unit);

private:
    /**
     * @brief Configures the texture's wrapping and filtering parameters.
     * Must be called while the texture is bound to its configured target.
     */
    void SetParameters() const;

    /** OpenGL texture object ID. Zero indicates that no texture object exists. */
    unsigned int Id = 0;
};
} // namespace N
