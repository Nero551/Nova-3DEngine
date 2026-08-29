#pragma once
#include <glslang/Public/ShaderLang.h>
#include "OpenGL.hpp"
#include "Utilities/Logger.hpp"

namespace N {
/** @brief Handles initialization and termination of graphics specific things
 * that need to run before window creation and everything else.
 * Created by Engine, constructed first, deconstructed last. that way it
 * prevents initialization errors.
 */
struct GraphicsContext {
    GraphicsContext() {
        glfwSetErrorCallback(
            [](const int error, const char* description) { U::Logger::Error("[GLFW]", error, ": ", description); });
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        if (!glslang::InitializeProcess()) {
            throw std::runtime_error("Failed to initialize glslang");
        }
    }

    ~GraphicsContext() {
        glslang::FinalizeProcess();
        glfwTerminate();
    }
};
} // namespace N
