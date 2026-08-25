#pragma once
#include "../../../vcpkg/buildtrees/glslang/src/16.4.0-0420a59a83.clean/glslang/MachineIndependent/LiveTraverser.h"
#include "OpenGL.hpp"
#include "Utilities/Logger.hpp"

namespace N {
/** @brief Handles initialization and termination of GLFW.
 * Created by Engine, constructed first, deconstructed last. that way it
 * prevents GLFW errors
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
