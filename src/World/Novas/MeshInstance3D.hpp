#pragma once
#include "Modules/Graphics/Components/MaterialComponent.hpp"
#include "Modules/Graphics/Components/MeshComponent.hpp"
#include "Nova3D.hpp"

namespace N
{
struct MeshInstance3D : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        C::World::Get().Query.Pool<G::MaterialComponent>().Add(GetId());
        C::World::Get().Query.Pool<G::MeshComponent>().Add(GetId());
    }
};
} // namespace N
