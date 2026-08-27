#include "Framebuffer.hpp"

namespace N {
Framebuffer::Framebuffer(const std::string& name) : Resource(name) {
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &Id);
}

void Framebuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glGenFramebuffers(1, &Id);
}

void Framebuffer::Regenerate() {
    glDeleteFramebuffers(1, &Id);
    Id = 0;
}

void Framebuffer::Bind() {
    Generate();
    glBindFramebuffer(static_cast<GLenum>(Target), Id);
}

bool Framebuffer::IsGenerated() const {
    return Id != 0;
}

bool Framebuffer::IsComplete() {
    Bind();
    if (glCheckFramebufferStatus(static_cast<GLenum>(Target)) == GL_FRAMEBUFFER_COMPLETE) {
        U::Logger::Info("FrameBuffer Usable, 🔥");
        return true;
    }
    U::Logger::Info("Framebuffer trash");
    return false;
}

void Framebuffer::AttachTexture(FramebufferAttachment textureAttachment, Texture& texture) {
    Bind();
    texture.Load();

    TextureAttachments.emplace(textureAttachment, &texture);
    glFramebufferTexture2D(static_cast<GLenum>(Target),
        static_cast<GLenum>(textureAttachment),
        static_cast<GLenum>(texture.Target),
        texture.GetId(),
        0);
}

void Framebuffer::AttachRenderBuffer(FramebufferAttachment attachment, Renderbuffer& renderbuffer) {
    Bind();
    renderbuffer.Generate();
    RenderBuffers.emplace(attachment, &renderbuffer);
    glFramebufferRenderbuffer(
        static_cast<GLenum>(Target), static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.GetId());
}

void Framebuffer::Resize(int width, int height) {
    Bind();

    for (auto& [attachment, texture] : TextureAttachments) {
        texture->Reload();
        texture->Height = height;
        texture->Width = width;
        texture->Load();
        AttachTexture(attachment, *texture);
    }

    for (auto& [attachment, buffer] : RenderBuffers) {
        buffer->Width = width;
        buffer->Height = height;
        buffer->Regenerate();
        buffer->Generate();

        AttachRenderBuffer(attachment, *buffer);
    }
}

unsigned int Framebuffer::GetId() const {
    return Id;
}
} // namespace N
