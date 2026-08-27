#pragma once
#include <vector>

#include "OpenGL.hpp"
#include "RenderBuffer.hpp"
#include "Resources/Texture/Texture.hpp"
#include "Utilities/CheckedPtr.hpp"
#include "Utilities/Logger.hpp"

namespace N {
enum class FrameBufferTarget { ReadDraw = GL_FRAMEBUFFER, Read = GL_READ_FRAMEBUFFER, Draw = GL_DRAW_FRAMEBUFFER };

enum class TextureAttachment {
    Color = GL_COLOR_ATTACHMENT0,
    Depth = GL_DEPTH_ATTACHMENT,
    Stencil = GL_STENCIL_ATTACHMENT,
    DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

struct Framebuffer {
    FrameBufferTarget Target = FrameBufferTarget::ReadDraw;
    std::unordered_map<TextureAttachment, U::CheckedPtr<Texture>> TextureAttachments;
    std::unordered_map<TextureAttachment, U::CheckedPtr<Renderbuffer>> RenderBuffers;

    Framebuffer() {
        glGenFramebuffers(1, &Id);
    }

    ~Framebuffer() {
        glDeleteFramebuffers(1, &Id);
    }

    void Bind() {
        glBindFramebuffer(static_cast<GLenum>(Target), Id);
    }

    bool IsComplete() {
        if (glCheckFramebufferStatus(static_cast<GLenum>(Target)) == GL_FRAMEBUFFER_COMPLETE) {
            U::Logger::Info("FrameBuffer Usable, 🔥");
            return true;
        }
        U::Logger::Info("Framebuffer trash");
        return false;
    }

    void AttachTexture(TextureAttachment textureAttachment, Texture& texture) {
        TextureAttachments.emplace(textureAttachment, &texture);
        glFramebufferTexture2D(static_cast<GLenum>(Target),
            static_cast<GLenum>(textureAttachment),
            static_cast<GLenum>(texture.Target),
            texture.GetId(),
            0);
    }

    void AttachRenderBuffer(TextureAttachment attachment, Renderbuffer& renderbuffer) {
        RenderBuffers.emplace(attachment, &renderbuffer);
        glFramebufferRenderbuffer(
            static_cast<GLenum>(Target), static_cast<GLenum>(attachment), GL_RENDERBUFFER, renderbuffer.GetId());
    }

    void Resize(int width, int height) {
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

        IsComplete();
    }

    unsigned int GetId() {
        return Id;
    }

private:
    unsigned int Id = 0;
};
} // namespace N
