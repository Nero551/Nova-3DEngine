#include "Uniformbuffer.hpp"

namespace N {
void Uniformbuffer::Generate() {
    if (IsGenerated()) {
        return;
    }

    glCreateBuffers(1, &Id);
    glNamedBufferData(Id, Size, nullptr, static_cast<GLenum>(Usage));
}

bool Uniformbuffer::IsGenerated() const {
    return Id != 0;
}

void Uniformbuffer::Regenerate() {
    glDeleteBuffers(1, &Id);
    Id = 0;
}

void Uniformbuffer::Bind() {
    Generate();
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, Id);
}

void Uniformbuffer::Unbind() {
    glBindBufferBase(GL_UNIFORM_BUFFER, Binding, 0);
}

unsigned int Uniformbuffer::GetId() const {
    return Id;
}
} // namespace N
