#pragma once
#include "Modules/Renderer/Components/MaterialComponent.hpp"
#include "Modules/Renderer/Components/MeshComponent.hpp"
#include "Nova3D.hpp"

namespace N {
struct MeshInstance3D : Nova3D {
    void Initialize() override {
        Nova3D::Initialize();
        AddComponents<MeshComponent, MaterialComponent>();
    }
};
} // namespace N
