#pragma once
#include "Modules/Renderer/Components/MaterialComponent.hpp"
#include "Modules/Renderer/Components/MeshComponent.hpp"
#include "Nova3D.hpp"

namespace N
{
struct MeshInstance3D : Nova3D
{
    void Initialize() override
    {
        Nova3D::Initialize();
        World::Get().Query.Pool<MaterialComponent>().Add(GetId());
        World::Get().Query.Pool<MeshComponent>().Add(GetId());
    }
};
} // namespace N
