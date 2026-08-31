#pragma once
#include "BufferUsage.hpp"
#include "OpenGL.hpp"

namespace N {
/** @brief OpenGL buffer for storing vertex and other array data. */
struct ArrayBuffer {
    /** @brief Controls the OpenGL usage hint applied when data is uploaded. */
    BufferUsage Usage = BufferUsage::StaticDraw;

    /** @brief Constructs an ungenerated array buffer. */
    ArrayBuffer();

    /** @brief Generates the buffer once; subsequent calls have no effect. */
    void Generate();

    /**
     * @brief Replaces the buffer's contents with the supplied array data.
     * automatically generates buffer if not generated.
     * @param data Elements to copy into the buffer.
     */
    template <typename T> void SetData(const std::vector<T>& data) {
        Generate();
        glNamedBufferData(Id, data.size() * sizeof(T), data.data(), static_cast<GLenum>(Usage));
    }

    /** @brief Returns true when a valid OpenGL buffer ID has been generated. */
    bool IsGenerated();

    /** @brief Releases the buffer and resets its ID to zero. */
    void Delete();

    /** @brief Releases the buffer when this object is destroyed. */
    ~ArrayBuffer();

    /** @brief Binds this buffer to GL_ARRAY_BUFFER. */
    void Bind();

    /** @brief Unbinds the current GL_ARRAY_BUFFER by binding buffer zero. */
    void Unbind();

    /** @brief Returns the underlying OpenGL buffer ID, or zero if ungenerated. */
    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
