#pragma once
#include "Modules/Graphics/Components/LightComponent.hpp"
#include "Nova3D.hpp"

namespace N
{
struct Light : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        C::World::Get().Query.Pool<LightComponent>().Add(GetId());
    }
};
} // namespace N
