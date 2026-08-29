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

    ArrayBuffer(const ArrayBuffer&) = delete;
    ArrayBuffer& operator=(const ArrayBuffer&) = delete;

    ArrayBuffer(ArrayBuffer&& other) noexcept;
    ArrayBuffer& operator=(ArrayBuffer&& other) noexcept;

    /** @brief Generates the buffer once; subsequent calls have no effect. */
    void Generate();

    /**
     * @brief Replaces the buffer's contents with the supplied array data.
     * @param data Elements to copy into the buffer.
     * Does nothing if the buffer has not been generated.
     * The buffer is temporarily bound during the upload and unbound afterward.
     */
    template <typename T> void SetData(const std::vector<T>& data) {
        if (!IsGenerated()) {
            Generate();
        }

        glBindBuffer(GL_ARRAY_BUFFER, Id);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), static_cast<GLenum>(Usage));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
