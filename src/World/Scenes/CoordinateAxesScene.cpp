#include "CoordinateAxesScene.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Grid.hpp"
#include "Math/Color/Color.hpp"
#include "Modules/Input/Enums/Keys.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Components/MaterialComponent.hpp"
#include "Modules/Renderer/Components/MeshComponent.hpp"
#include "Modules/Renderer/Primitives/Primitives.hpp"
#include "World/Novas/Camera.hpp"
#include "World/Novas/Light.hpp"
#include "World/Novas/MeshInstance3D.hpp"
#include "World/Novas/Nova3D.hpp"

namespace E {
CoordinateAxesScene::CoordinateAxesScene() {
    auto& resourceManager = Service::Get<ResourceManager>();

    SetRoot(World::Get().CreateEntity<Nova>());

    auto& lightShader = resourceManager.Load<Shader>("lightShader");
    lightShader.AssignSource(
        resourceManager.Load<ShaderSource>("lightFrag", "Assets/Shaders/lightShader.frag", ShaderStage::Fragment));
    lightShader.AssignSource(
        resourceManager.Load<ShaderSource>("lightVert", "Assets/Shaders/lightShader.vert", ShaderStage::Vertex));

    auto& lightMaterial = resourceManager.Load<Material>("lightMaterial");
    lightMaterial.Shader = &lightShader;

    auto& light = World::Get().CreateEntity<Light>();
    light.GetComponent<Transform3DComponent>().Rotation = M::Quaternion::FromEulerXYZ({ M::Rad(32.5) });
    GetRoot().AttachChild(light);

    auto& light2 = World::Get().CreateEntity<Light>();
    light2.GetComponent<Transform3DComponent>().Rotation = M::Quaternion::FromEulerXYZ({ M::Rad(-32.5) });
    GetRoot().AttachChild(light2);

    auto& shader = Service::Get<ResourceManager>().Load<Shader>("AxisShader");
    shader.AssignSource(resourceManager.Load<ShaderSource>("axisFrag", "Assets/Shaders/axisShader.frag", ShaderStage::Fragment));
    shader.AssignSource(resourceManager.Load<ShaderSource>("axisVert", "Assets/Shaders/axisShader.vert", ShaderStage::Vertex));

    auto& line = Primitives::CreateLine("Line");

    auto& xAxis = World::Get().CreateEntity<MeshInstance3D>();
    xAxis.GetComponent<MeshComponent>().Mesh = &line;
    xAxis.GetComponent<MaterialComponent>().Material = &resourceManager.Load<Material>("X-Axis Material");
    xAxis.GetComponent<MaterialComponent>().Material->Shader = &shader;

    xAxis.GetComponent<Transform3DComponent>().Rotation = M::Quaternion::FromEulerXYZ({ 0, M::Rad(90), 0 });
    xAxis.GetComponent<Transform3DComponent>().Scale = { 1, 1, 200 };
    xAxis.GetComponent<MaterialComponent>().Material->Color = M::Color::Red;

    GetRoot().AttachChild(xAxis);

    auto& yAxis = World::Get().CreateEntity<MeshInstance3D>();
    yAxis.GetComponent<MeshComponent>().Mesh = &line;
    yAxis.GetComponent<MaterialComponent>().Material = &resourceManager.Load<Material>("Y-Axis Material");
    yAxis.GetComponent<MaterialComponent>().Material->Shader = &shader;

    yAxis.GetComponent<Transform3DComponent>().Rotation = M::Quaternion::FromEulerXYZ({ M::Rad(-90), 0, 0 });
    yAxis.GetComponent<Transform3DComponent>().Scale = { 1, 1, 200 };
    yAxis.GetComponent<MaterialComponent>().Material->Color = M::Color::Green;

    GetRoot().AttachChild(yAxis);

    auto& zAxis = World::Get().CreateEntity<MeshInstance3D>();
    zAxis.GetComponent<MeshComponent>().Mesh = &line;
    zAxis.GetComponent<MaterialComponent>().Material = &resourceManager.Load<Material>("Z-Axis Material");
    zAxis.GetComponent<MaterialComponent>().Material->Shader = &shader;

    zAxis.GetComponent<Transform3DComponent>().Scale = { 1, 1, 200 };
    zAxis.GetComponent<MaterialComponent>().Material->Color = M::Color::Blue;

    U::Logger::Info(zAxis.GetComponent<Transform3DComponent>().Rotation.ToMatrix4());

    GetRoot().AttachChild(zAxis);

    Grid grid;
    GetRoot().AttachChild(grid.GetRoot());
}
} // namespace E
