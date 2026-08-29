#pragma once
#include "BufferUsage.hpp"
#include "OpenGL.hpp"

namespace N {
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
} // namespace N
