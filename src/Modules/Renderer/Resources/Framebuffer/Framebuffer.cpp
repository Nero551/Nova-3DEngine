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
    glCreateFramebuffers(1, &Id);
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

bool Framebuffer::IsComplete() const {
    const GLenum status = glCheckNamedFramebufferStatus(Id, static_cast<GLenum>(Target));

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        U::Logger::Error("Framebuffer: " + Name + " is not complete");
        return false;
    }

    return true;
}

void Framebuffer::AttachTexture(FramebufferAttachment textureAttachment, Texture& texture) {
    Generate();
    texture.Generate();
    TextureAttachments.emplace(textureAttachment, &texture);
    glNamedFramebufferTexture(Id, static_cast<GLenum>(textureAttachment), texture.GetId(), 0);
}

void Framebuffer::Blit(Framebuffer& dst, int srcW, int srcH, int dstW, int dstH) {
    if (!IsGenerated()) {
        Generate();
    }
    if (!dst.IsGenerated()) {
        dst.Generate();
    }

    glBlitNamedFramebuffer(Id, dst.Id, 0, 0, srcW, srcH, 0, 0, dstW, dstH, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::AttachRenderBuffer(FramebufferAttachment attachment, Renderbuffer& renderbuffer) {
    Generate();
    renderbuffer.Generate();
    RenderBuffers.emplace(attachment, &renderbuffer);
    glNamedFramebufferRenderbuffer(Id, static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.GetId());
}

void Framebuffer::Resize(int width, int height) {
    for (auto& [attachment, texture] : TextureAttachments) {
        texture->Regenerate();
        texture->Width = width;
        texture->Height = height;
        texture->Generate();

        glNamedFramebufferTexture(Id, static_cast<GLenum>(attachment), texture->GetId(), 0);
    }

    for (auto& [attachment, buffer] : RenderBuffers) {
        buffer->Regenerate();
        buffer->Width = width;
        buffer->Height = height;
        buffer->Generate();

        glNamedFramebufferRenderbuffer(Id, static_cast<GLenum>(attachment), GL_RENDERBUFFER, buffer->GetId());
    }
}

unsigned int Framebuffer::GetId() const {
    return Id;
}
} // namespace N
