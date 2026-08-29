#pragma once
#include "ArrayBuffer.hpp"

namespace N {
/** @brief OpenGL element buffer containing unsigned integer indices for indexed drawing. */
struct IndexBuffer {
    /** @brief Controls the OpenGL usage hint applied when index data is uploaded. */
    BufferUsage Usage = BufferUsage::StaticDraw;

    /** @brief Constructs an ungenerated index buffer. */
    IndexBuffer();

    /** @brief Generates the buffer once; subsequent calls have no effect. */
    void Generate();

    /**
     * @brief Replaces the buffer's contents with the supplied index data.
     * @param indices Unsigned integer indices used by indexed drawing.
     * Does nothing if the buffer has not been generated.
     * The buffer is temporarily bound during the upload and unbound afterward.
     */
    void SetData(const std::vector<unsigned int>& indices);

    /** @brief Returns true when a valid OpenGL buffer ID has been generated. */
    bool IsGenerated();

    /** @brief Releases the buffer and resets its ID to zero. */
    void Delete();

    /** @brief Releases the buffer when this object is destroyed. */
    ~IndexBuffer();

    /** @brief Binds this buffer to GL_ELEMENT_ARRAY_BUFFER for indexed drawing. */
    void Bind();

    /** @brief Unbinds the current GL_ELEMENT_ARRAY_BUFFER by binding buffer zero. */
    void Unbind();

    /** @brief Returns the underlying OpenGL buffer ID, or zero if ungenerated. */
    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
