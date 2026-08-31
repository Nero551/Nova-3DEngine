#include "Texture2D.hpp"
#include "OpenGL.hpp"

#include "Utilities/Logger.hpp"

namespace N {
void Texture2D::Generate() {
    Texture::Generate();
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

void Texture2D::UseImage(const U::Image& image) {
    Width = image.Width;
    Height = image.Height;
    Data = image.Pixels;
    DataType = DataType::UnsignedByte;
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

void Texture2D::SetParameters() const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(WrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(MinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(MagFilter));
}
} // namespace N
