#pragma once
#include "Modules/Renderer/Components/CameraComponent.hpp"
#include "Nova3D.hpp"

namespace N {
struct Camera : Nova3D {
    void Initialize() override {
        Nova3D::Initialize();
        AddComponent<CameraComponent>();
    }
};
} // namespace N
