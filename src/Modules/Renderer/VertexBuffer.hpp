#pragma once
#include "DataType.hpp"
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

    template <typename T> void Generate(const std::vector<T>& data) {
        if (IsGenerated()) {
            return;
        }
        glGenBuffers(1, &Id);
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

    void Generate(const std::vector<unsigned int>& indices) {
        if (IsGenerated()) {
            return;
        }

        glGenBuffers(1, &Id);
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
    ArrayBuffer VBO;
    IndexBuffer EBO;

    void Generate(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        if (IsGenerated()) {
            return;
        }
        glGenVertexArrays(1, &Id);
        glBindVertexArray(Id);
        VBO.Generate(vertices);
        EBO.Generate(indices);
    }

    bool IsGenerated() {
        return Id != 0;
    }

    void Delete() {
        VBO.Delete();
        EBO.Delete();
        glDeleteVertexArrays(1, &Id);
        Id = 0;
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &Id);
    }

    void Bind() {
        glBindVertexArray(Id);
        VBO.Bind();
        EBO.Bind();
    }

    void Unbind() {
        glBindVertexArray(0);
        VBO.Unbind();
        EBO.Unbind();
    }

    void SetAttribPointer(int index, int size, DataType type, size_t stride, size_t offset, bool normalized = false) {
        glVertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));
        glEnableVertexAttribArray(index);
    }

    unsigned int GetId() const {
        return Id;
    }

private:
    unsigned int Id = 0;
};
} // namespace N
