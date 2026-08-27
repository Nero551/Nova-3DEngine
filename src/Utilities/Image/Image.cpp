#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.hpp"

#include <stb_image.h>
#include <stb_image_write.h>

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

void Image::SaveToDiskPNG(const std::string& filepath, bool flip) {
    stbi_flip_vertically_on_write(flip);
    stbi_write_png(
        filepath.c_str(), Width, Height, static_cast<size_t>(Channels), Pixels.data(), Width * static_cast<size_t>(Channels));
}

void Image::FlipVertically() {
    const size_t rowSize = static_cast<size_t>(Width) * static_cast<size_t>(Channels);

    for (int y = 0; y < Height / 2; ++y) {
        auto top = Pixels.begin() + static_cast<size_t>(y) * rowSize;
        auto bottom = Pixels.begin() + static_cast<size_t>(Height - 1 - y) * rowSize;

        std::swap_ranges(top, top + rowSize, bottom);
    }
}
} // namespace N::U
