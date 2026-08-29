#include "ArrayBuffer.hpp"

namespace N {
ArrayBuffer::ArrayBuffer() {
}

void ArrayBuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glGenBuffers(1, &Id);
}

bool ArrayBuffer::IsGenerated() {
    return Id != 0;
}

void ArrayBuffer::Delete() {
    glDeleteBuffers(1, &Id);
    Id = 0;
}

ArrayBuffer::ArrayBuffer(ArrayBuffer&& other) noexcept : Id(std::exchange(other.Id, 0)) {
}

ArrayBuffer& ArrayBuffer::operator=(ArrayBuffer&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    Delete();
    Id = std::exchange(other.Id, 0);
    return *this;
}

ArrayBuffer::~ArrayBuffer() {
    glDeleteBuffers(1, &Id);
}

void ArrayBuffer::Bind() {
    if (!IsGenerated()) {
        Generate();
    }
    glBindBuffer(GL_ARRAY_BUFFER, Id);
}

void ArrayBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int ArrayBuffer::GetId() const {
    return Id;
}
} // namespace N
