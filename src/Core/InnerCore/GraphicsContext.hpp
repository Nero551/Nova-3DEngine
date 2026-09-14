#pragma once
#include "OpenGL.hpp"
#include "Utilities/Log.hpp"
#include <glslang/Public/ShaderLang.h>

namespace N
{
/** @brief Handles initialization and termination of graphics specific things
 * that need to run before window creation and everything else.
 * Created by Engine, constructed first, deconstructed last. that way it
 * prevents initialization errors.
 */
struct GraphicsContext
{
    GraphicsContext()
    {
        glfwSetErrorCallback([](const int error, const char* description)
            { U::Log::Error("[GLFW]", error, ": ", description); });

        if (!glfwInit())
        {
            U::Log::Fatal("Failed to initialize GLFW");
        }

        if (!glslang::InitializeProcess())
        {
            U::Log::Fatal("Failed to initialize glslang");
        }
    }

    ~GraphicsContext()
    {
        glslang::FinalizeProcess();
        glfwTerminate();
    }
};
} // namespace N
