#pragma once
#include "Core/OuterCore/Resource.hpp"
#include "OpenGL.hpp"
#include "Resources/Texture/TextureInternalFormat.hpp"

namespace N {
struct Renderbuffer : Resource {
    TextureInternalFormat InternalFormat = TextureInternalFormat::Depth24Stencil8;
    int Width = 0;
    int Height = 0;

    Renderbuffer(const std::string& name, TextureInternalFormat internalFormat = TextureInternalFormat::Depth24Stencil8,
        int width = 0, int height = 0) : Resource(name), InternalFormat(internalFormat), Width(width), Height(height) {
    }

    bool IsGenerated() {
        return Id != 0;
    }

    void Generate() {
        if (IsGenerated()) {
            return;
        }
        glGenRenderbuffers(1, &Id);
        glBindRenderbuffer(GL_RENDERBUFFER, Id);
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(InternalFormat), Width, Height);
    }

    void Regenerate() {
        glDeleteRenderbuffers(1, &Id);
        Id = 0;
    }

    void Bind() {
        Generate();
        glBindRenderbuffer(GL_RENDERBUFFER, Id);
    }

    unsigned int GetId() {
        return Id;
    }

private:
    unsigned int Id = 0;
};
} // namespace N
