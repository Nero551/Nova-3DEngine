#include "Grid.hpp"

#include "Core/InnerCore/World.hpp"
#include "Core/OuterCore/Service.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"
#include "Math/Common/Trigonometry.hpp"
#include "Modules/Graphics/Primitives/Primitives.hpp"
#include "Modules/Graphics/Resources/Shader/Shader.hpp"
#include "Modules/Graphics/Resources/Shader/ShaderSource.hpp"
#include "World/Novas/MeshInstance3D.hpp"
#include "World/Novas/Nova.hpp"

namespace N
{
Grid::Grid()
{
    SetRoot(C::World::Get().CreateEntity<Nova>());

    CreateXY();
    // CreateXZ();
    // CreateYZ();

    // for (int theta = 0; theta <= 360; ++theta) {
    // for (int phi = 0; phi <= 360; ++phi) {
    // }
    // float r = 3;
    // CreateGridLine({M::Rad(theta), 0, 0}, 0);
    // CreateGridLine(M::Quaternion::FromEulerXYZ({M::Rad(theta), M::Rad(theta),
    // M::Rad(theta)}), 0);
}

void Grid::CreateGridLine(const M::Quaternion rotation, const M::Vector3 position)
{
    auto& resourceManager = C::Service::Get<C::ResourceManager>();
    auto& shader = C::Service::Get<C::ResourceManager>().Load<Shader>("AxisShader");
    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "axisFrag", "Assets/Shaders/axisShader.frag", ShaderStage::Fragment));
    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "axisVert", "Assets/Shaders/axisShader.vert", ShaderStage::Vertex));
    auto& line = Primitives::CreateLine("Line");

    auto& l = C::World::Get().CreateEntity<MeshInstance3D>();
    auto& materialComponent = C::World::Get().Query.Pool<MaterialComponent>().GetComponentById(l.GetId());
    auto& meshComponent = C::World::Get().Query.Pool<MeshComponent>().GetComponentById(l.GetId());
    auto& transformComponent = C::World::Get().Query.Pool<Transform3DComponent>().GetComponentById(l.GetId());
    meshComponent.Mesh = &line;
    materialComponent.Material = &resourceManager.Load<Material>("GridLine Material");
    materialComponent.Material->Shader = &shader;

    transformComponent.Rotation = rotation;
    transformComponent.Position = position;
    transformComponent.Scale = {1, 1, 40};
    materialComponent.Material->Color = M::Color::Gray;
    GetRoot().AttachChild(l);
}

void Grid::CreateXY()
{
    for (float x = -20; x < 20; ++x)
    {
        if (x != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({M::Rad(90), 0, 0}), {x, 0, 0});
        }
    }

    for (float y = -20; y < 20; ++y)
    {
        if (y != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({0, M::Rad(90), 0}), {0, y, 0});
        }
    }
}

void Grid::CreateXZ()
{
    for (float x = -20; x < 20; ++x)
    {
        if (x != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({0, 0, M::Rad(90)}), {x, 0, 0});
        }
    }

    for (float z = -20; z < 20; ++z)
    {
        if (z != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({M::Rad(90), 0, M::Rad(90)}), {0, 0, z});
        }
    }
}

void Grid::CreateYZ()
{
    for (float y = -20; y < 20; ++y)
    {
        if (y != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({0, 0, 0}), {0, y, 0});
        }
    }

    for (float z = -20; z < 20; ++z)
    {
        if (z != 0)
        {
            CreateGridLine(M::Quaternion::FromEulerXYZ({M::Rad(90), 0, 0}), {0, 0, z});
        }
    }
}
} // namespace N
