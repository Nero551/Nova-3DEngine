#pragma once

namespace N::U {
/**
 * @brief Represents a raster image .
 *
 * Stores the raw pixel data along with the image dimensions
 * and number of color channels.
 */
struct Image {
    enum class ColorChannels { R = 1, RG = 2, RGB = 3, RGBA = 4 };

    /**
     * @brief Loads an image from disk.
     * @param filePath Path to the image file.
     * @param flip Whether to vertically flip the image when loading.
     */
    Image(const std::string& filePath, bool flip = false);

    /**
     * @brief Creates an image from raw pixel data.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     * @param channels Number of color channels per pixel.
     * @param pixels vector containing each pixel's color data.
     */
    Image(int width, int height, ColorChannels channels, const std::vector<unsigned char>& pixels);

    std::vector<unsigned char> Pixels{};

    /** Image width in pixels. */
    int Width = 0;

    /** Image height in pixels. */
    int Height = 0;

    /** Color channel configuration of the image. */
    ColorChannels Channels = ColorChannels::R;
};
} // namespace N::U
