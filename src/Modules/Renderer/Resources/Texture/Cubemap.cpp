#include "Cubemap.hpp"

#include "Utilities/Logger.hpp"

namespace N {
void Cubemap::Generate() {
    Texture::Generate();
    if (IsGenerated()) {
        return;
    }

    Width = Front.Width;
    Height = Front.Height;

    if (Back.Width != Width || Back.Height != Height || Top.Width != Width || Top.Height != Height || Bottom.Width != Width ||
        Bottom.Height != Height || Right.Width != Width || Right.Height != Height || Left.Width != Width ||
        Left.Height != Height) {
        U::Logger::Error("Cubemap faces must all have the same dimensions.");
        return;
    }

    glGenTextures(1, &Id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
    SetParameters();

    GLint intern = static_cast<GLint>(InternalFormat);
    GLenum format = static_cast<GLenum>(Format);
    GLenum dataType = static_cast<GLenum>(DataType);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, intern, Width, Height, 0, format, dataType, Front.Pixels.data());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, intern, Width, Height, 0, format, dataType, Back.Pixels.data());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, intern, Width, Height, 0, format, dataType, Top.Pixels.data());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, intern, Width, Height, 0, format, dataType, Bottom.Pixels.data());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, intern, Width, Height, 0, format, dataType, Right.Pixels.data());

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, intern, Width, Height, 0, format, dataType, Left.Pixels.data());

    if (AutoMipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::SetParameters() const {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(WrapS));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(WrapT));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(MinFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(MagFilter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLenum>(WrapR));
}
} // namespace N
