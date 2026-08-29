#pragma once
#include "DataType.hpp"
#include "Math/Matrix/Matrix4.hpp"
#include "OpenGL.hpp"
#include "Resources/Mesh/Vertex.hpp"

namespace N {
/**
 * @brief Specifies how a buffer's data will be used.
 */
enum class BufferUsage : GLenum {
    StreamDraw = GL_STREAM_DRAW,
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,

    StreamRead = GL_STREAM_READ,
    StaticRead = GL_STATIC_READ,
    DynamicRead = GL_DYNAMIC_READ,

    StreamCopy = GL_STREAM_COPY,
    StaticCopy = GL_STATIC_COPY,
    DynamicCopy = GL_DYNAMIC_COPY
};

enum class BufferTarget : GLenum {
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    Uniform = GL_UNIFORM_BUFFER,
    ShaderStorage = GL_SHADER_STORAGE_BUFFER,
    DrawIndirect = GL_DRAW_INDIRECT_BUFFER
};

struct ArrayBuffer {
    BufferUsage Usage = BufferUsage::StaticDraw;
    ArrayBuffer() {
    }

    void Generate() {
        if (IsGenerated()) {
            return;
        }
        glGenBuffers(1, &Id);
    }

    template <typename T> void SetData(const std::vector<T>& data) {
        if (!IsGenerated()) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, Id);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), static_cast<GLenum>(Usage));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    bool IsGenerated() {
        return Id != 0;
    }

    void Delete() {
        glDeleteBuffers(1, &Id);
        Id = 0;
    }

    ~ArrayBuffer() {
        glDeleteBuffers(1, &Id);
    }

    void Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, Id);
    }

    void Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    unsigned int GetId() const {
        return Id;
    }

private:
    unsigned int Id = 0;
};

struct IndexBuffer {
    BufferUsage Usage = BufferUsage::StaticDraw;

    IndexBuffer() {
    }

    void Generate() {
        if (IsGenerated()) {
            return;
        }
        glGenBuffers(1, &Id);
    }

    void SetData(const std::vector<unsigned int>& indices) {
        if (!IsGenerated()) {
            return;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), static_cast<GLenum>(Usage));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    bool IsGenerated() {
        return Id != 0;
    }

    void Delete() {
        glDeleteBuffers(1, &Id);
        Id = 0;
    }

    ~IndexBuffer() {
        glDeleteBuffers(1, &Id);
    }

    void Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
    }

    void Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int GetId() const {
        return Id;
    }

private:
    unsigned int Id = 0;
};

struct VertexArray {
    void Generate() {
        if (IsGenerated()) {
            return;
        }
        glGenVertexArrays(1, &Id);
    }

    bool IsGenerated() {
        return Id != 0;
    }

    void Delete() {
        glDeleteVertexArrays(1, &Id);
        Id = 0;
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &Id);
    }

    void Bind() {
        glBindVertexArray(Id);
    }

    void Unbind() {
        glBindVertexArray(0);
    }

    void SetAttribPointer(int index, int size, DataType type, size_t stride, size_t offset, bool normalized = false) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));
    }

    void SetMatrix3AttribPointer(int startIndex) {
        SetAttribPointer(startIndex, 3, DataType::Float, sizeof(M::Matrix3), 0);
        SetAttribPointer(startIndex + 1, 3, DataType::Float, sizeof(M::Matrix3), sizeof(M::Vector3));
        SetAttribPointer(startIndex + 2, 3, DataType::Float, sizeof(M::Matrix3), 2 * sizeof(M::Vector3));
    }

    void SetMatrix4AttribPointer(int startIndex) {
        SetAttribPointer(startIndex, 4, DataType::Float, sizeof(M::Matrix4), 0);
        SetAttribPointer(startIndex + 1, 4, DataType::Float, sizeof(M::Matrix4), sizeof(M::Vector4));
        SetAttribPointer(startIndex + 2, 4, DataType::Float, sizeof(M::Matrix4), 2 * sizeof(M::Vector4));
        SetAttribPointer(startIndex + 3, 4, DataType::Float, sizeof(M::Matrix4), 3 * sizeof(M::Vector4));
    }

    void SetAttribDivisor(int index, int divisor) {
        glVertexAttribDivisor(index, divisor);
    }

    void SetMatrix3AttribDivisor(int startIndex, int divisor) {
        SetAttribDivisor(startIndex, divisor);
        SetAttribDivisor(startIndex + 1, divisor);
        SetAttribDivisor(startIndex + 2, divisor);
    }

    void SetMatrix4AttribDivisor(int startIndex, int divisor) {
        SetAttribDivisor(startIndex, divisor);
        SetAttribDivisor(startIndex + 1, divisor);
        SetAttribDivisor(startIndex + 2, divisor);
        SetAttribDivisor(startIndex + 3, divisor);
    }

    unsigned int GetId() const {
        return Id;
    }

private:
    unsigned int Id = 0;
};
} // namespace N
