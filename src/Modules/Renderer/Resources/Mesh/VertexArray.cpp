#include "VertexArray.hpp"

#include "Buffers/ArrayBuffer.hpp"
#include "Buffers/IndexBuffer.hpp"

namespace N {
VertexArray::VertexArray() {
}

void VertexArray::Generate() {
    if (IsGenerated()) {
        return;
    }
    glCreateVertexArrays(1, &Id);
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
    Generate();
    glBindVertexArray(Id);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}

void VertexArray::SetVertexBuffer(const ArrayBuffer& vbo, int bindingIndex, int stride, int offset) {
    Generate();
    glVertexArrayVertexBuffer(Id, bindingIndex, vbo.GetId(), offset, stride);
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ebo) {
    Generate();
    glVertexArrayElementBuffer(Id, ebo.GetId());
}

void VertexArray::SetAttribPointer(int index, int size, DataType type, int bindingIndex, size_t offset, bool normalized) {
    Generate();
    glEnableVertexArrayAttrib(Id, index);
    glVertexArrayAttribBinding(Id, index, bindingIndex);
    glVertexArrayAttribFormat(Id, index, size, static_cast<GLenum>(type), normalized, offset);
}

void VertexArray::SetMatrix3AttribPointer(int startIndex, int stride, int offset, int bindingIndex) {
    SetAttribPointer(startIndex, 3, DataType::Float, bindingIndex, 0 + offset);
    SetAttribPointer(startIndex + 1, 3, DataType::Float, bindingIndex, sizeof(M::Vector3) + offset);
    SetAttribPointer(startIndex + 2, 3, DataType::Float, bindingIndex, 2 * sizeof(M::Vector3) + offset);
}

void VertexArray::SetMatrix4AttribPointer(int startIndex, int stride, int offset, int bindingIndex) {
    SetAttribPointer(startIndex, 4, DataType::Float, bindingIndex, 0 + offset);
    SetAttribPointer(startIndex + 1, 4, DataType::Float, bindingIndex, sizeof(M::Vector4) + offset);
    SetAttribPointer(startIndex + 2, 4, DataType::Float, bindingIndex, 2 * sizeof(M::Vector4) + offset);
    SetAttribPointer(startIndex + 3, 4, DataType::Float, bindingIndex, 3 * sizeof(M::Vector4) + offset);
}

void VertexArray::SetAttribDivisor(int bindingIndex, int divisor) {
    Generate();
    glVertexArrayBindingDivisor(Id, bindingIndex, divisor);
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
