#include "Texture.hpp"
#include "OpenGL.hpp"

namespace N {
Texture::Texture(const std::string& name, TextureTarget target) : Resource(name), Target(target) {
}

Texture::~Texture() {
    glDeleteTextures(1, &Id);
}

TextureTarget Texture::GetTarget() const {
    return Target;
}

unsigned int Texture::GetId() const {
    return Id;
}

void Texture::Regenerate() {
    glDeleteTextures(1, &Id);
    Id = 0;
}

bool Texture::IsGenerated() const {
    return Id != 0;
}

void Texture::Generate() {
}

void Texture::Bind(const unsigned int unit) {
    if (!IsGenerated()) {
        Generate();
    }

    glBindTextureUnit(unit, Id);
}

void Texture::Unbind(unsigned int unit) const {
    glBindTextureUnit(unit, 0);
}
} // namespace N
