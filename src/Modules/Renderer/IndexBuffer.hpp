#pragma once
#include "ArrayBuffer.hpp"
#include "OpenGL.hpp"

namespace N {
/** @brief OpenGL buffer containing vertex indices for indexed drawing. */
struct IndexBuffer {
    /** @brief Specifies how the buffer's data will be used. */
    BufferUsage Usage = BufferUsage::StaticDraw;

    /** @brief Constructs an ungenerated index buffer. */
    IndexBuffer();

    /** @brief Generates the OpenGL buffer object. */
    void Generate();

    /** @brief Uploads index data to the buffer. */
    void SetData(const std::vector<unsigned int>& indices);

    /** @brief Returns whether the buffer has been generated. */
    bool IsGenerated();

    /** @brief Deletes the OpenGL buffer object. */
    void Delete();

    /** @brief Deletes the OpenGL buffer object on destruction. */
    ~IndexBuffer();

    /** @brief Binds the buffer to the element array target. */
    void Bind();

    /** @brief Unbinds the buffer from the element array target. */
    void Unbind();

    /** @brief Returns the OpenGL buffer object ID. */
    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
