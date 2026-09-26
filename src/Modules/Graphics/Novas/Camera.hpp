#pragma once
#include "../../../World/Novas/Nova3D.hpp"
#include "Modules/Graphics/Components/CameraComponent.hpp"

namespace N::G
{
struct Camera : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        C::World::Get().Query.Pool<G::CameraComponent>().Add(GetId());
    }
};
} // namespace N::G
