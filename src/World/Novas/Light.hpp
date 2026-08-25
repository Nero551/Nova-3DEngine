#pragma once
#include "Modules/Renderer/Components/LightComponent.hpp"
#include "Nova3D.hpp"

namespace N {
struct Light : Nova3D {
    void Initialize() override {
        Nova3D::Initialize();
        AddComponents<LightComponent>();
    }
};
} // namespace N
