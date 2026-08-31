#include "ArrayBuffer.hpp"

namespace N {
ArrayBuffer::ArrayBuffer() {
}

void ArrayBuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glCreateBuffers(1, &Id);
}

bool ArrayBuffer::IsGenerated() {
    return Id != 0;
}

void ArrayBuffer::Delete() {
    glDeleteBuffers(1, &Id);
    Id = 0;
}

ArrayBuffer::~ArrayBuffer() {
    glDeleteBuffers(1, &Id);
}

void ArrayBuffer::Bind() {
    Generate();
    glBindBuffer(GL_ARRAY_BUFFER, Id);
}

void ArrayBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int ArrayBuffer::GetId() const {
    return Id;
}
} // namespace N
