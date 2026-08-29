#include "IndexBuffer.hpp"

namespace N {
IndexBuffer::IndexBuffer() {
}

void IndexBuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glGenBuffers(1, &Id);
}

void IndexBuffer::SetData(const std::vector<unsigned int>& indices) {
    if (!IsGenerated()) {
        Generate();
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), static_cast<GLenum>(Usage));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool IndexBuffer::IsGenerated() {
    return Id != 0;
}

void IndexBuffer::Delete() {
    glDeleteBuffers(1, &Id);
    Id = 0;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &Id);
}

void IndexBuffer::Bind() {
    if (!IsGenerated()) {
        Generate();
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetId() const {
    return Id;
}
} // namespace N
