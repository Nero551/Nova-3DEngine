#pragma once

#include "Core/InnerCore/Module.hpp"
#include "Enums/Keys.hpp"
#include "Enums/Mouse.hpp"
#include "Math/Vector/Vector2.hpp"

namespace N {
struct Input : Module {
    bool IsKeyHeld(Key key);

    bool IsKeyPressed(Key key);

    bool IsKeyReleased(Key key);

    bool IsMouseButtonHeld(MouseButton button);

    bool IsMouseButtonPressed(MouseButton button);

    bool IsMouseButtonReleased(MouseButton button);

    M::Vector2 GetMousePosition() const;

    M::Vector2 GetMouseDelta() const;

    M::Vector2 GetScrollDelta() const;

    bool IsScrolling() const;

    MouseMode GetMouseMode() const;

    void SetMouseMode(MouseMode mode);

private:
    MouseMode MouseMode = MouseMode::Normal;
    M::Vector2 MousePosition = M::Vector2::Zero;
    M::Vector2 PreviousMousePosition = M::Vector2::Zero;
    bool FirstMouse = true;

    M::Vector2 ScrollOffset = M::Vector2::Zero;

    static constexpr unsigned int MouseButtonCount = GLFW_MOUSE_BUTTON_LAST + 1;
    static constexpr unsigned int KeyCount = GLFW_KEY_LAST + 1;

    constexpr unsigned int ToIndex(Key key);

    constexpr unsigned int ToIndex(MouseButton button);

    std::array<bool, KeyCount> CurrentKeys{};
    std::array<bool, KeyCount> PreviousKeys{};

    std::array<bool, MouseButtonCount> CurrentMouseButtons{};
    std::array<bool, MouseButtonCount> PreviousMouseButtons{};

protected:
    void Start() override;

    void BeginFrame(double dt) override;

    void EndFrame(double dt) override;
};
} // namespace N
