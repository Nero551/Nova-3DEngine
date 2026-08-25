#include "Input.hpp"

#include <OpenGL.hpp>

#include "Core/InnerCore/Engine.hpp"

namespace N {
bool Input::IsKeyHeld(const Key key) {
    return CurrentKeys[ToIndex(key)];
}

bool Input::IsKeyPressed(const Key key) {
    return CurrentKeys[ToIndex(key)] && !PreviousKeys[ToIndex(key)];
}

bool Input::IsKeyReleased(const Key key) {
    return !CurrentKeys[ToIndex(key)] && PreviousKeys[ToIndex(key)];
}

bool Input::IsMouseButtonHeld(const MouseButton button) {
    return CurrentMouseButtons[ToIndex(button)];
}

bool Input::IsMouseButtonPressed(const MouseButton button) {
    return CurrentMouseButtons[ToIndex(button)] && !PreviousMouseButtons[ToIndex(button)];
}

bool Input::IsMouseButtonReleased(const MouseButton button) {
    return !CurrentMouseButtons[ToIndex(button)] && PreviousMouseButtons[ToIndex(button)];
}

M::Vector2 Input::GetMousePosition() const {
    return MousePosition;
}

M::Vector2 Input::GetMouseDelta() const {
    return GetMousePosition() - PreviousMousePosition;
}

M::Vector2 Input::GetScrollDelta() const {
    return ScrollOffset;
}

bool Input::IsScrolling() const {
    return ScrollOffset != M::Vector2::Zero;
}

MouseMode Input::GetMouseMode() const {
    return MouseMode;
}

void Input::SetMouseMode(enum MouseMode mode) {
    MouseMode = mode;
    glfwSetInputMode(Engine::Get().Window.GetGlfwWindow(), GLFW_CURSOR, static_cast<int>(mode));
}

constexpr unsigned int Input::ToIndex(Key key) {
    return static_cast<unsigned int>(key);
}

constexpr unsigned int Input::ToIndex(MouseButton button) {
    return static_cast<unsigned int>(button);
}

void Input::OnStart() {
    Window& window = Engine::Get().Window;
    glfwSetCursorPosCallback(window.GetGlfwWindow(), [](GLFWwindow*, const double xPos, const double yPos) {
        Engine::Get().GetModule<Input>().MousePosition = { static_cast<float>(xPos), static_cast<float>(yPos) };
    });

    glfwSetScrollCallback(window.GetGlfwWindow(), [](GLFWwindow*, const double xOffset, const double yOffset) {
        Engine::Get().GetModule<Input>().ScrollOffset = { static_cast<float>(xOffset), static_cast<float>(yOffset) };
    });
}

void Input::OnBeginFrame(double dt) {
    if (FirstMouse) {
        PreviousMousePosition = GetMousePosition();
        FirstMouse = false;
    }

    GLFWwindow* window = Engine::Get().Window.GetGlfwWindow();
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
        CurrentKeys[key] = glfwGetKey(window, key) == GLFW_PRESS;
    }

    for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button) {
        CurrentMouseButtons[button] = glfwGetMouseButton(window, button);
    }
}

void Input::OnEndFrame(double dt) {
    ScrollOffset = { 0, 0 };
    PreviousMousePosition = MousePosition;
    PreviousKeys = CurrentKeys;
    PreviousMouseButtons = CurrentMouseButtons;
}
} // namespace N
