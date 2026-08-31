#include "IndexBuffer.hpp"

namespace N {
IndexBuffer::IndexBuffer() {
}

void IndexBuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glCreateBuffers(1, &Id);
}

void IndexBuffer::SetData(const std::vector<unsigned int>& indices) {
    Generate();
    glNamedBufferData(Id, indices.size() * sizeof(unsigned int), indices.data(), static_cast<GLenum>(Usage));
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
    Generate();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetId() const {
    return Id;
}
} // namespace N
