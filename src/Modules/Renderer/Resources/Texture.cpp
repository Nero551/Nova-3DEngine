#include "Texture.hpp"

#include "OpenGL.hpp"
#include "Utilities/Image/Image.hpp"
#include "Utilities/Logger.hpp"

namespace N {
Texture::Texture(const std::string& name, const U::Image& image) : Resource(name) {
    glGenTextures(1, &Id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Id);

    SetParameters();

    GLenum format = GL_RED;

    switch (image.Channels) {
    case U::Image::ColorChannels::R:
        format = GL_RED;
        break;
    case U::Image::ColorChannels::RG:
        format = GL_RG;
        break;
    case U::Image::ColorChannels::RGB:
        format = GL_RGB;
        break;
    case U::Image::ColorChannels::RGBA:
        format = GL_RGBA;
        break;
    default:
        U::Logger::Error("Unsupported Texture Channel Count");
    }

    glTexImage2D(GL_TEXTURE_2D,
        0,
        static_cast<GLint>(format),
        image.Width,
        image.Height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image.Pixels.data());

    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &Id);
}

unsigned int Texture::GetId() const {
    return Id;
}

void Texture::Bind(const unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, Id);
}

void Texture::SetParameters() const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
} // namespace N
