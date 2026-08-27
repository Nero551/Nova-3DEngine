#include "Texture.hpp"

#include "OpenGL.hpp"
#include "Utilities/Image/Image.hpp"
#include "Utilities/Logger.hpp"

namespace N {
Texture::Texture(const std::string& name) : Resource(name) {
}

Texture::~Texture() {
    glDeleteTextures(1, &Id);
}

void Texture::UseImage(const U::Image& image) {
    Width = image.Width;
    Height = image.Height;
    Data = image.Pixels;
    DataType = TextureDataType::UnsignedByte;
    Target = TextureTarget::Texture2D;
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

unsigned int Texture::GetId() const {
    return Id;
}

void Texture::Reload() {
    glDeleteTextures(1, &Id);
    Id = 0;
}

bool Texture::IsLoaded() const {
    return GetId() != 0;
}

void Texture::Load() {
    if (IsLoaded()) {
        return;
    }

    glGenTextures(1, &Id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(static_cast<GLenum>(Target), Id);
    SetParameters();

    glTexImage2D(static_cast<GLenum>(Target),
        0,
        static_cast<GLint>(InternalFormat),
        Width,
        Height,
        0,
        static_cast<GLenum>(Format),
        static_cast<GLenum>(DataType),
        Data.data());

    if (AutoMipmaps) {
        glGenerateMipmap(static_cast<GLenum>(Target));
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(static_cast<GLenum>(Target), 0);
}

void Texture::Bind(const unsigned int unit) {
    if (!IsLoaded()) {
        Load();
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(static_cast<GLenum>(Target), Id);
}

void Texture::SetParameters() const {
    glTexParameteri(static_cast<GLenum>(Target), GL_TEXTURE_WRAP_S, static_cast<GLint>(WrapS));
    glTexParameteri(static_cast<GLenum>(Target), GL_TEXTURE_WRAP_T, static_cast<GLint>(WrapT));
    glTexParameteri(static_cast<GLenum>(Target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(MinFilter));
    glTexParameteri(static_cast<GLenum>(Target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(MagFilter));
}

void Texture::LoadCubemap() {
}
} // namespace N
