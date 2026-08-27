#include "Window.hpp"

#include "Engine.hpp"
#include "Utilities/Logger.hpp"

namespace N {
void Window::SetHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHintString(GLFW_X11_CLASS_NAME, "nova_engine");
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, "nova_engine");
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}

Window::Window(const int width, const int height, const std::string& title) {
    SetHints();

    GLFWwindow* glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!glfwWindow) {
        U::Logger::Fatal("Failed To Create Window");
    }
    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        U::Logger::Fatal("Failed To Initialize GLAD");
    }

    U::Logger::Info(glGetString(GL_VERSION));

    glViewport(0, 0, width, height);
    GlfwWindow = glfwWindow;
}


Window::~Window() {
    glfwDestroyWindow(GlfwWindow);
}

float Window::GetAspectRatio() const {
    return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(GlfwWindow);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(GlfwWindow);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SetTitle(const std::string& title) {
    glfwSetWindowTitle(GlfwWindow, title.c_str());
}

void Window::SetIcon(const U::Image& icon) {
    GLFWimage image;
    image.height = icon.Height;
    image.width = icon.Width;
    image.pixels = const_cast<unsigned char*>(icon.Pixels.data());
    glfwSetWindowIcon(GlfwWindow, 1, &image);
}

void Window::SetHeight(const int height) {
    glfwSetWindowSize(GlfwWindow, GetWidth(), height);
}

void Window::SetWidth(const int width) {
    glfwSetWindowSize(GlfwWindow, width, GetHeight());
}

void Window::SetSize(const int width, const int height) {
    SetWidth(width);
    SetHeight(height);
}

int Window::GetHeight() const {
    int height = 0;
    int width = 0;
    glfwGetWindowSize(GlfwWindow, &width, &height);
    return height;
}

int Window::GetWidth() const {
    int height = 0;
    int width = 0;
    glfwGetWindowSize(GlfwWindow, &width, &height);
    return width;
}

void Window::Close() {
    glfwSetWindowShouldClose(GlfwWindow, GL_TRUE);
}

GLFWwindow* Window::GetGlfwWindow() {
    return GlfwWindow;
}
} // namespace N
