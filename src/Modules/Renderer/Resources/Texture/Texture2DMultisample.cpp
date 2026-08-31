#include "Texture2DMultisample.hpp"

namespace N {
void Texture2DMultisample::Generate() {
    Texture::Generate();
    if (IsGenerated()) {
        return;
    }

    glGenTextures(1, &Id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, Id);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Samples, static_cast<GLint>(InternalFormat), Width, Height, GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}
} // namespace N
