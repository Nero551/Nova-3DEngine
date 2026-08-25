#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Math/Matrix/Matrix4.hpp"

namespace N {
struct CameraSystem : System {
    void FixedUpdate(double fdt) override;
    M::Matrix4 GetViewMatrix();
};
} // namespace N
