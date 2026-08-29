#include "Uniformbuffer.hpp"

namespace N {
void Uniformbuffer::Generate() {
    glGenBuffers(1, &Id);
    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBufferData(GL_UNIFORM_BUFFER, Size, nullptr, static_cast<GLenum>(Usage));
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, Id);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

bool Uniformbuffer::IsGenerated() const {
    return Id != 0;
}

void Uniformbuffer::Regenerate() {
    glDeleteBuffers(1, &Id);
    Id = 0;
}

void Uniformbuffer::Bind() {
    if (!IsGenerated()) {
        Generate();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, Id);
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, Id);
}

void Uniformbuffer::Unbind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned int Uniformbuffer::GetId() const {
    return Id;
}
} // namespace N
