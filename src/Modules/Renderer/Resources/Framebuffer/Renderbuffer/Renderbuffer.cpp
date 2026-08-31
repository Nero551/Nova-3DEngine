#include "Renderbuffer.hpp"

namespace N {
Renderbuffer::Renderbuffer(const std::string& name) : Resource(name) {
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &Id);
}

bool Renderbuffer::IsGenerated() const {
    return Id != 0;
}

void Renderbuffer::Generate() {
    if (IsGenerated()) {
        return;
    }
    glCreateRenderbuffers(1, &Id);

    if (Samples == 0) {
        glNamedRenderbufferStorage(Id, static_cast<GLenum>(InternalFormat), Width, Height);
    }
    else {
        glNamedRenderbufferStorageMultisample(Id, Samples, static_cast<GLenum>(InternalFormat), Width, Height);
    }
}

void Renderbuffer::Regenerate() {
    glDeleteRenderbuffers(1, &Id);
    Id = 0;
}

void Renderbuffer::Bind() {
    Generate();
    glBindRenderbuffer(GL_RENDERBUFFER, Id);
}

unsigned int Renderbuffer::GetId() const {
    return Id;
}
} // namespace N
