#pragma once
#include "Modules/Graphics/Components/CameraComponent.hpp"
#include "Nova3D.hpp"

namespace N
{
struct Camera : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        C::World::Get().Query.Pool<G::CameraComponent>().Add(GetId());
    }
};
} // namespace N
