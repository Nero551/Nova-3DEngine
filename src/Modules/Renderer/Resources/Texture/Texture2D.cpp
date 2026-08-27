#include "Texture2D.hpp"

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
} // namespace N
