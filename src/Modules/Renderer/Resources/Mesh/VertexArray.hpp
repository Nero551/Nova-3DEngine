#pragma once
#include <cstddef>

#include "../../DataType.hpp"
#include "Buffers/IndexBuffer.hpp"
#include "Math/Matrix/Matrix4.hpp"
#include "OpenGL.hpp"

namespace N {
/** @brief OpenGL vertex array object that stores vertex attribute configuration. */
struct VertexArray {
    /** @brief Constructs an ungenerated vertex array object. */
    VertexArray();

    /** @brief Generates the VAO once; subsequent calls have no effect. */
    void Generate();

    /** @brief Returns true when a valid OpenGL VAO ID has been generated. */
    bool IsGenerated();

    /** @brief Releases the VAO and resets its ID to zero. */
    void Delete();

    /** @brief Releases the VAO when this object is destroyed. */
    ~VertexArray();

    /** @brief Binds this VAO, making its stored vertex attribute state current. */
    void Bind();

    /** @brief Binds VAO 0, restoring the default vertex array state. */
    void Unbind();
    void SetVertexBuffer(const ArrayBuffer& vbo, int bindingIndex, int stride, int offset = 0);

    void SetIndexBuffer(const IndexBuffer& ebo);
    /**
     * @brief Enables a vertex attribute and describes its layout in the bound buffer.
     * @param index Attribute location.
     * @param size Number of components per attribute.
     * @param type Component data type.
     * @param bindingIndex
     * @param offset Byte offset of the first attribute.
     * @param normalized Converts integer data to normalized floating-point values when true.
     */
    void SetAttribPointer(int index, int size, DataType type, int bindingIndex, size_t offset, bool normalized = false);

    /**
     * @brief Maps a Matrix3 to three consecutive vertex attributes, one per column.
     * @param startIndex Attribute location of the first column.
     * @param offset
     * @param bindingIndex
     */
    void SetMatrix3AttribPointer(int startIndex, int offset, int bindingIndex);

    /**
     * @brief Maps a Matrix4 to four consecutive vertex attributes, one per column.
     * @param startIndex Attribute location of the first column.
     * @param offset
     * @param bindingIndex
     */
    void SetMatrix4AttribPointer(int startIndex, int offset, int bindingIndex);

    /**
     * @brief Controls how often an attribute advances when using instanced rendering.
     * @param bindingIndex Attribute location.
     * @param divisor Zero advances per vertex; non-zero advances once every divisor instances.
     */
    void SetAttribDivisor(int bindingIndex, int divisor);

    /** @brief Returns the underlying OpenGL VAO ID, or zero if ungenerated. */
    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
