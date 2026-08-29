#pragma once
#include "DataType.hpp"
#include "Math/Matrix/Matrix4.hpp"
#include "OpenGL.hpp"

namespace N {
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
