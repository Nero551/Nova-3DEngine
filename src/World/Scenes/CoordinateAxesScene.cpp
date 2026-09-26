#include "CoordinateAxesScene.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Grid.hpp"
#include "Math/Color/Color.hpp"
#include "Modules/Graphics/Components/MaterialComponent.hpp"
#include "Modules/Graphics/Components/MeshComponent.hpp"
#include "Modules/Graphics/Primitives/Primitives.hpp"
#include "World/Novas/Camera.hpp"
#include "World/Novas/Light.hpp"
#include "World/Novas/MeshInstance3D.hpp"
#include "World/Novas/Nova3D.hpp"

namespace N
{
CoordinateAxesScene::CoordinateAxesScene()
{
    auto& world = C::World::Get();
    auto& query = world.Query;
    auto& resourceManager = C::Service::Get<C::ResourceManager>();

    SetRoot(world.CreateEntity<Nova>());

    auto& lightShader = resourceManager.Load<Shader>("lightShader");
    lightShader.AssignSource(resourceManager.Load<ShaderSource>(
        "lightFrag", "Assets/Shaders/lightShader.frag", ShaderStage::Fragment));
    lightShader.AssignSource(resourceManager.Load<ShaderSource>(
        "lightVert", "Assets/Shaders/lightShader.vert", ShaderStage::Vertex));

    auto& lightMaterial = resourceManager.Load<Material>("lightMaterial");
    lightMaterial.Shader = &lightShader;

    auto& light = world.CreateEntity<Light>();
    query.Pool<Transform3DComponent>().GetComponentById(light.GetId()).Rotation =
        M::Quaternion::FromEulerXYZ(M::Vector3{M::Rad(32.5)});
    GetRoot().AttachChild(light);

    auto& light2 = world.CreateEntity<Light>();
    query.Pool<Transform3DComponent>().GetComponentById(light2.GetId()).Rotation =
        M::Quaternion::FromEulerXYZ(M::Vector3{M::Rad(-32.5)});
    GetRoot().AttachChild(light2);

    auto& shader = resourceManager.Load<Shader>("AxisShader");
    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "axisFrag", "Assets/Shaders/axisShader.frag", ShaderStage::Fragment));
    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "axisVert", "Assets/Shaders/axisShader.vert", ShaderStage::Vertex));

    auto& line = Primitives::CreateLine("Line");

    auto& xAxis = world.CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(xAxis.GetId()).Mesh = &line;

    auto& xMaterial = query.Pool<MaterialComponent>().GetComponentById(xAxis.GetId());
    xMaterial.Material = &resourceManager.Load<Material>("X-Axis Material");
    xMaterial.Material->Shader = &shader;

    query.Pool<Transform3DComponent>().GetComponentById(xAxis.GetId()).Rotation =
        M::Quaternion::FromEulerXYZ({0, M::Rad(90), 0});
    query.Pool<Transform3DComponent>().GetComponentById(xAxis.GetId()).Scale = {1, 1, 200};
    xMaterial.Material->Color = M::Color::Red;

    GetRoot().AttachChild(xAxis);

    auto& yAxis = world.CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(yAxis.GetId()).Mesh = &line;

    auto& yMaterial = query.Pool<MaterialComponent>().GetComponentById(yAxis.GetId());
    yMaterial.Material = &resourceManager.Load<Material>("Y-Axis Material");
    yMaterial.Material->Shader = &shader;

    query.Pool<Transform3DComponent>().GetComponentById(yAxis.GetId()).Rotation =
        M::Quaternion::FromEulerXYZ({M::Rad(-90), 0, 0});
    query.Pool<Transform3DComponent>().GetComponentById(yAxis.GetId()).Scale = {1, 1, 200};
    yMaterial.Material->Color = M::Color::Green;

    GetRoot().AttachChild(yAxis);

    auto& zAxis = world.CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(zAxis.GetId()).Mesh = &line;

    auto& zMaterial = query.Pool<MaterialComponent>().GetComponentById(zAxis.GetId());
    zMaterial.Material = &resourceManager.Load<Material>("Z-Axis Material");
    zMaterial.Material->Shader = &shader;

    query.Pool<Transform3DComponent>().GetComponentById(zAxis.GetId()).Scale = {1, 1, 200};
    zMaterial.Material->Color = M::Color::Blue;

    GetRoot().AttachChild(zAxis);

    Grid grid;
    GetRoot().AttachChild(grid.GetRoot());
}
} // namespace N
