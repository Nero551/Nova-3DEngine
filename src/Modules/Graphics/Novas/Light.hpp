#pragma once
#include "../../../World/Novas/Nova3D.hpp"
#include "Modules/Graphics/Components/LightComponent.hpp"

namespace N::G
{
struct Light : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        C::World::Get().Query.Pool<G::LightComponent>().Add(GetId());
    }
};
} // namespace N::G
