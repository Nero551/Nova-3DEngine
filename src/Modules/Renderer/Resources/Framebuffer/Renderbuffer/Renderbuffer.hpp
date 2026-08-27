#pragma once

#include "../../Texture/TextureInternalFormat.hpp"
#include "Core/OuterCore/Resource.hpp"
#include "OpenGL.hpp"

namespace N {
/**
 * @brief Represents an OpenGL renderbuffer resource.
 *
 * A renderbuffer provides storage for renderable data such as color, depth,
 * or stencil information. Renderbuffers are primarily intended for use as
 * framebuffer attachments and cannot be directly sampled by shaders.
 */
struct Renderbuffer : Resource {
    /** @brief Internal format used to store the renderbuffer's data. */
    TextureInternalFormat InternalFormat = TextureInternalFormat::Depth24Stencil8;

    /** @brief Width of the renderbuffer in pixels. */
    int Width = 0;

    /** @brief Height of the renderbuffer in pixels. */
    int Height = 0;

    /**
     * @brief Creates a renderbuffer resource.
     *
     * The underlying OpenGL renderbuffer is not generated until Generate()
     * or Bind() is called.
     *
     * @param name Resource name.
     */
    Renderbuffer(const std::string& name);

    /**
     * @brief Releases the underlying OpenGL renderbuffer object.
     */
    ~Renderbuffer() override;

    /**
     * @brief Checks whether the renderbuffer has been generated.
     *
     * @return true if an OpenGL renderbuffer object exists, otherwise false.
     */
    bool IsGenerated() const;

    /**
     * @brief Generates the OpenGL renderbuffer object.
     *
     * Allocates renderbuffer storage using the current internal format,
     * width, and height. Does nothing if the renderbuffer is already generated.
     */
    void Generate();

    /**
     * @brief Deletes the current OpenGL renderbuffer object.
     *
     * The renderbuffer can be generated again afterward with Generate().
     */
    void Regenerate();

    /**
     * @brief Binds the renderbuffer to GL_RENDERBUFFER.
     *
     * Automatically generates the renderbuffer if it has not been generated.
     */
    void Bind();

    /**
     * @brief Gets the OpenGL renderbuffer object ID.
     *
     * @return OpenGL renderbuffer ID, or 0 if the renderbuffer has not been generated.
     */
    unsigned int GetId() const;

private:
    /// OpenGL renderbuffer object ID. Zero indicates that no renderbuffer exists.
    unsigned int Id = 0;
};
} // namespace N
