#pragma once
#include <vector>

#include "Texture.hpp"
#include "Utilities/Image/Image.hpp"
#include "Utilities/Logger.hpp"

namespace N {
struct Texture2D : Texture {
    /** CPU-side pixel data used when loading the texture to the GPU. */
    std::vector<unsigned char> Data;


    Texture2D(const std::string& name) : Texture(name, TextureTarget::Texture2D) {
    }

    void Generate() override {
        if (IsGenerated()) {
            return;
        }

        glGenTextures(1, &Id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Id);
        SetParameters();

        glTexImage2D(GL_TEXTURE_2D,
            0,
            static_cast<GLint>(InternalFormat),
            Width,
            Height,
            0,
            static_cast<GLenum>(Format),
            static_cast<GLenum>(DataType),
            Data.data());

        if (AutoMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    /**
     * @brief Replaces the texture's data and configuration with the supplied image's data.
     *
     * Updates the texture dimensions, pixel data, source format, and internal
     * format using the supplied image. Existing GPU resources are not reloaded.
     *
     * @param image Image to use for the texture.
     * @remark The image should be vertically flipped when loaded to account
     * for the difference between image and OpenGL texture coordinates.
     */
    void UseImage(const U::Image& image) {
        Width = image.Width;
        Height = image.Height;
        Data = image.Pixels;
        DataType = TextureDataType::UnsignedByte;
        AutoMipmaps = true;

        switch (image.Channels) {
        case U::Image::ColorChannels::R:
            Format = TextureFormat::Red;
            InternalFormat = TextureInternalFormat::R8;
            break;
        case U::Image::ColorChannels::RG:
            Format = TextureFormat::RG;
            InternalFormat = TextureInternalFormat::RG8;
            break;
        case U::Image::ColorChannels::RGB:
            Format = TextureFormat::RGB;
            InternalFormat = TextureInternalFormat::RGB8;
            break;
        case U::Image::ColorChannels::RGBA:
            Format = TextureFormat::RGBA;
            InternalFormat = TextureInternalFormat::RGBA8;
            break;
        default:
            U::Logger::Error("Unsupported Texture Channel Count");
        }
    }
};
} // namespace N
