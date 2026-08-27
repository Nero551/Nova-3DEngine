#pragma once
#include <vector>

#include "../Texture/Texture.hpp"
#include "FramebufferAttachment.hpp"
#include "FramebufferTarget.hpp"
#include "OpenGL.hpp"
#include "Renderbuffer/Renderbuffer.hpp"
#include "Utilities/CheckedPtr.hpp"
#include "Utilities/Logger.hpp"

namespace N {
/**
 * @brief Represents an OpenGL framebuffer object.
 *
 * A framebuffer provides a collection of attachment points that determine
 * where rendered color, depth, and stencil data is stored. Attachments may
 * be provided by textures or renderbuffers.
 *
 * Framebuffers are generated lazily through Generate() and can be recreated
 * with Regenerate(). A framebuffer must be complete before it can be used
 * as a rendering target.
 */
struct Framebuffer : Resource {
    /**
     * @brief Specifies which framebuffer target is affected by operations.
     *
     * Defaults to the read/draw framebuffer target, which is suitable for
     * normal rendering.
     */
    FrameBufferTarget Target = FrameBufferTarget::ReadDraw;

    /**
     * @brief Textures currently attached to this framebuffer.
     *
     * The key identifies the framebuffer attachment point and the value
     * references the texture attached to it.
     */
    std::unordered_map<FramebufferAttachment, U::CheckedPtr<Texture>> TextureAttachments;

    /**
     * @brief Renderbuffers currently attached to this framebuffer.
     *
     * The key identifies the framebuffer attachment point and the value
     * references the renderbuffer attached to it.
     */
    std::unordered_map<FramebufferAttachment, U::CheckedPtr<Renderbuffer>> RenderBuffers;

    /**
     * @brief Creates a framebuffer resource.
     *
     * @param name Resource name.
     */
    Framebuffer(const std::string& name);

    /// Releases the underlying OpenGL framebuffer object.
    ~Framebuffer() override;

    /**
     * @brief Generates the OpenGL framebuffer object.
     *
     * Does nothing if the framebuffer has already been generated.
     */
    void Generate();

    /**
     * @brief Deletes the current OpenGL framebuffer object.
     *
     * The framebuffer can be generated again afterward with Generate().
     */
    void Regenerate();

    /**
     * @brief Binds this framebuffer to its configured target.
     */
    void Bind();

    /** @brief Unbinds this framebuffer from the configured target. sets the bound to default framebuffer */
    void Unbind();

    /**
     * @brief Checks whether the OpenGL framebuffer object has been generated.
     *
     * @return true if an OpenGL framebuffer object exists, otherwise false.
     */
    bool IsGenerated() const;

    /**
     * @brief Checks whether the framebuffer is complete.
     *
     * A framebuffer is complete when all required attachments and their
     * configurations satisfy OpenGL's framebuffer completeness rules.
     *
     * @return true if the framebuffer is complete, otherwise false.
     */
    bool IsComplete();

    /**
     * @brief Attaches a texture to a framebuffer attachment point.
     *
     * @param textureAttachment Attachment point to use.
     * @param texture Texture to attach.
     */
    void AttachTexture(FramebufferAttachment textureAttachment, Texture& texture);

    /**
     * @brief Attaches a renderbuffer to a framebuffer attachment point.
     *
     * @param attachment Attachment point to use.
     * @param renderbuffer Renderbuffer to attach.
     */
    void AttachRenderBuffer(FramebufferAttachment attachment, Renderbuffer& renderbuffer);

    /**
     * @brief Resizes all attachments of the framebuffer.
     *
     * Recreates attached textures and renderbuffers using the specified
     * dimensions, then reattaches them to the framebuffer.
     *
     * @param width New attachment width in pixels.
     * @param height New attachment height in pixels.
     */
    void Resize(int width, int height);

    /**
     * @brief Gets the OpenGL framebuffer object ID.
     *
     * @return OpenGL framebuffer ID, or 0 if the framebuffer has not been generated.
     */
    unsigned int GetId() const;

private:
    /// OpenGL framebuffer object ID. Zero indicates that no framebuffer exists.
    unsigned int Id = 0;
};
} // namespace N
