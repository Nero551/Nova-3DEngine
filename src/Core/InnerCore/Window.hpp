#pragma once
#include <OpenGL.hpp>

#include "Utilities/Image/Image.hpp"

namespace N {
/**
 * @brief Wrapper around a GLFW window.
 * Owns the underlying GLFW window and provides basic window
 * management and event handling.
 */
struct Window {
    /**
     * @brief Creates a window.
     * @param width Window width in pixels.
     * @param height Window height in pixels.
     * @param title Window title.
     */
    Window(int width, int height, const std::string& title);

    /** @brief Destroys the underlying GLFW window. */
    ~Window();

    /** @brief Returns the window's width-to-height ratio. */
    [[nodiscard]] float GetAspectRatio() const;

    /**
     * @brief Checks whether the window has been requested to close.
     * @return True if the window should close.
     */
    bool ShouldClose();

    /** Swaps the front and back buffers. */
    void SwapBuffers();

    /** Processes pending GLFW events. */
    void PollEvents();

    void SetTitle(const std::string& title);

    void SetIcon(const U::Image& icon);

    /** @param height New height in pixels */
    void SetHeight(int height);

    /** @param width New width in pixels */
    void SetWidth(int width);

    /**
     * @brief Changes the window dimensions.
     * @param width New width in pixels.
     * @param height New height in pixels.
     */
    void SetSize(int width, int height);

    /** @brief Returns height in pixels */
    int GetHeight() const;

    /** @brief Returns width in pixels */
    int GetWidth() const;

    void Close();

    GLFWwindow* GetGlfwWindow();

private:
    GLFWwindow* GlfwWindow;

    void SetHints();
};
} // namespace N
