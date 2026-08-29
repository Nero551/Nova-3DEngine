#include "VertexArray.hpp"

namespace N {
VertexArray::VertexArray() {
}

void VertexArray::Generate() {
    if (IsGenerated()) {
        return;
    }
    glGenVertexArrays(1, &Id);
}

bool VertexArray::IsGenerated() {
    return Id != 0;
}

void VertexArray::Delete() {
    glDeleteVertexArrays(1, &Id);
    Id = 0;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &Id);
}

void VertexArray::Bind() {
    if (!IsGenerated()) {
        Generate();
    }
    glBindVertexArray(Id);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}

void VertexArray::SetAttribPointer(int index, int size, DataType type, size_t stride, size_t offset, bool normalized) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void*>(offset));
}

void VertexArray::SetMatrix3AttribPointer(int startIndex) {
    SetAttribPointer(startIndex, 3, DataType::Float, sizeof(M::Matrix3), 0);
    SetAttribPointer(startIndex + 1, 3, DataType::Float, sizeof(M::Matrix3), sizeof(M::Vector3));
    SetAttribPointer(startIndex + 2, 3, DataType::Float, sizeof(M::Matrix3), 2 * sizeof(M::Vector3));
}

void VertexArray::SetMatrix4AttribPointer(int startIndex) {
    SetAttribPointer(startIndex, 4, DataType::Float, sizeof(M::Matrix4), 0);
    SetAttribPointer(startIndex + 1, 4, DataType::Float, sizeof(M::Matrix4), sizeof(M::Vector4));
    SetAttribPointer(startIndex + 2, 4, DataType::Float, sizeof(M::Matrix4), 2 * sizeof(M::Vector4));
    SetAttribPointer(startIndex + 3, 4, DataType::Float, sizeof(M::Matrix4), 3 * sizeof(M::Vector4));
}

void VertexArray::SetAttribDivisor(int index, int divisor) {
    glVertexAttribDivisor(index, divisor);
}


void VertexArray::SetMatrix3AttribDivisor(int startIndex, int divisor) {
    SetAttribDivisor(startIndex, divisor);
    SetAttribDivisor(startIndex + 1, divisor);
    SetAttribDivisor(startIndex + 2, divisor);
}

void VertexArray::SetMatrix4AttribDivisor(int startIndex, int divisor) {
    SetAttribDivisor(startIndex, divisor);
    SetAttribDivisor(startIndex + 1, divisor);
    SetAttribDivisor(startIndex + 2, divisor);
    SetAttribDivisor(startIndex + 3, divisor);
}

unsigned int VertexArray::GetId() const {
    return Id;
}
} // namespace N
