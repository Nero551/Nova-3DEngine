#pragma once

namespace N::U {
/**
 * @brief Represents a raster image.
 *
 * Stores raw pixel data along with the image dimensions
 * and number of color channels.
 */
struct Image {
    /**
     * @brief Specifies the number of color channels in each pixel.
     */
    enum class ColorChannels {
        /** Single red channel. */
        R = 1,

        /** Red and green channels. */
        RG = 2,

        /** Red, green, and blue channels. */
        RGB = 3,

        /** Red, green, blue, and alpha channels. */
        RGBA = 4
    };

    Image() {
    }

    /**
     * @brief Loads an image from disk.
     *
     * @param filePath Path to the image file.
     * @param flip Whether to vertically flip the image when loading.
     */
    Image(const std::string& filePath, bool flip = false);

    /**
     * @brief Creates an image from raw pixel data.
     *
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     * @param channels Number of color channels per pixel.
     * @param pixels Raw pixel data.
     */
    Image(int width, int height, ColorChannels channels, const std::vector<unsigned char>& pixels);

    /**
     * @brief Saves the image to disk as a PNG file.
     *
     * @param filepath Destination path for the PNG file.
     * @param flip Whether to vertically flip the image when writing.
     */
    void SaveToDiskPNG(const std::string& filepath, bool flip = false);

    /**
     * @brief Vertically flips the image in place.
     *
     * Swaps the top and bottom rows of pixels. This is useful when
     * converting between coordinate systems with different vertical
     * origins, such as OpenGL's bottom-left origin and conventional
     * image formats' top-left origin.
     */
    void FlipVertically();

    /** Raw pixel data stored in CPU memory. */
    std::vector<unsigned char> Pixels{};

    /** Image width in pixels. */
    int Width = 0;

    /** Image height in pixels. */
    int Height = 0;

    /** Number and configuration of color channels per pixel. */
    ColorChannels Channels = ColorChannels::RGB;
};
} // namespace N::U
