#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Math/Matrix/Matrix4.hpp"

namespace N
{
struct CameraSystem : C::System
{
    void Update(double dt) override;
    M::Matrix4 GetViewMatrix();
};
} // namespace N
