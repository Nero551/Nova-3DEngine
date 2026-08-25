#define STB_IMAGE_IMPLEMENTATION
#include "Image.hpp"

#include <stb_image.h>

#include "Utilities/Logger.hpp"

namespace N::U {
Image::Image(const std::string& filePath, const bool flip) {
    stbi_set_flip_vertically_on_load(flip);

    int nrChannels = 1;
    unsigned char* pixels = stbi_load(filePath.c_str(), &Width, &Height, &nrChannels, 0);

    Channels = static_cast<enum ColorChannels>(nrChannels);

    if (!pixels) {
        Logger::Error("Failed To Load Image: " + filePath);
        return;
    }

    const size_t size = static_cast<size_t>(Width) * static_cast<size_t>(Height) * static_cast<size_t>(Channels);

    Pixels.assign(pixels, pixels + size);

    stbi_image_free(pixels);
}

Image::Image(const int width, const int height, const ColorChannels channels, const std::vector<unsigned char>& pixels) {
    Width = width;
    Height = height;
    Channels = channels;
    Pixels = pixels;
}
} // namespace N::U
