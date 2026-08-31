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
    glGenRenderbuffers(1, &Id);
    glBindRenderbuffer(GL_RENDERBUFFER, Id);

    if (Samples == 0) {
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(InternalFormat), Width, Height);
    }
    else {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, Samples, static_cast<GLenum>(InternalFormat), Width, Height);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
