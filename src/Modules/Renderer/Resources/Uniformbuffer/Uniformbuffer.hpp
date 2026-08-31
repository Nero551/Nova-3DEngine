#pragma once
#include "Core/OuterCore/Resource.hpp"
#include "Modules/Renderer/Resources/Mesh/Buffers/ArrayBuffer.hpp"
#include "OpenGL.hpp"

namespace N {
/**
 * @brief Represents an OpenGL Uniform Buffer Object.
 */
struct Uniformbuffer : Resource {
    // TODO: Store uniforms and calculate their offsets automatically.
    // Values currently have to be written to the buffer manually.

    /** @brief Specifies how the buffer's data will be used. */
    BufferUsage Usage = BufferUsage::StaticDraw;

    /** @brief Size of the buffer in bytes. */
    unsigned int Size = 0;

    /** @brief Uniform buffer binding point. */
    unsigned int Binding = 0;

    Uniformbuffer(const std::string& name) : Resource(name) {
    }

    /** @brief Generates the OpenGL uniform buffer. */
    void Generate();

    /**
     * @brief Writes a value to the buffer at the specified byte offset.
     * @param value Value to write.
     * @param offset Byte offset within the buffer.
     * @param extraSize Additional bytes to write for alignment or padding.
     */
    template <typename T> void Set(const T& value, int offset, int extraSize = 0) {
        glNamedBufferSubData(Id, offset, sizeof(T) + extraSize, &value);
    }

    /** @brief Checks whether the uniform buffer has been generated. */
    bool IsGenerated() const;

    /** @brief Deletes the OpenGL uniform buffer. */
    void Regenerate();

    /** @brief Binds the buffer to its uniform buffer binding point. */
    void Bind();

    /** @brief Unbinds the buffer from its uniform buffer binding point. */
    void Unbind();

    /** @brief Gets the OpenGL uniform buffer object ID. */
    unsigned int GetId() const;

private:
    /** OpenGL uniform buffer object ID. */
    unsigned int Id = 0;
};
} // namespace N
