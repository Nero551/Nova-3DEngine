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

void Framebuffer::Unbind() {
    glBindFramebuffer(static_cast<GLenum>(Target), 0);
}

bool Framebuffer::IsGenerated() const {
    return Id != 0;
}

bool Framebuffer::IsComplete() {
    Bind();
    if (glCheckFramebufferStatus(static_cast<GLenum>(Target)) == GL_FRAMEBUFFER_COMPLETE) {
        Unbind();
        return true;
    }
    if (glCheckFramebufferStatus(static_cast<GLenum>(Target)) != GL_FRAMEBUFFER_COMPLETE) {
        U::Logger::Error("Framebuffer: " + Name + " is not complete");
    }
    Unbind();
    return false;
}

void Framebuffer::AttachTexture(FramebufferAttachment textureAttachment, Texture& texture) {
    Bind();
    texture.Generate();

    TextureAttachments.emplace(textureAttachment, &texture);
    glFramebufferTexture2D(static_cast<GLenum>(Target),
        static_cast<GLenum>(textureAttachment),
        static_cast<GLenum>(texture.GetTarget()),
        texture.GetId(),
        0);

    Unbind();
}

void Framebuffer::Blit(Framebuffer& dst, int srcW, int srcH, int dstW, int dstH) {
    if (!IsGenerated()) {
        Generate();
    }
    if (!dst.IsGenerated()) {
        dst.Generate();
    }

    glBindFramebuffer(static_cast<GLenum>(FrameBufferTarget::Read), Id);
    glBindFramebuffer(static_cast<GLenum>(FrameBufferTarget::Draw), dst.Id);
    glBlitFramebuffer(0, 0, srcW, srcH, 0, 0, dstW, dstH, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AttachRenderBuffer(FramebufferAttachment attachment, Renderbuffer& renderbuffer) {
    Bind();
    renderbuffer.Generate();
    RenderBuffers.emplace(attachment, &renderbuffer);
    glFramebufferRenderbuffer(
        static_cast<GLenum>(Target), static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.GetId());

    Unbind();
}

void Framebuffer::Resize(int width, int height) {
    Bind();

    for (auto& [attachment, texture] : TextureAttachments) {
        texture->Regenerate();
        texture->Height = height;
        texture->Width = width;
        texture->Generate();
        AttachTexture(attachment, *texture);
    }

    for (auto& [attachment, buffer] : RenderBuffers) {
        buffer->Regenerate();
        buffer->Width = width;
        buffer->Height = height;
        buffer->Generate();

        AttachRenderBuffer(attachment, *buffer);
    }
    Unbind();
}

unsigned int Framebuffer::GetId() const {
    return Id;
}
} // namespace N
