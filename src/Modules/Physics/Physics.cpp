#include "Physics.hpp"

#include "Components/BodyComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"
#include "Math/Common/Constraints.hpp"
#include "Math/Common/Exponentials.hpp"
#include "Math/Common/Trigonometry.hpp"
#include "Math/Functions/Function.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Components/MaterialComponent.hpp"
#include "Modules/Renderer/Components/MeshComponent.hpp"
#include "Modules/Renderer/Primitives/Primitives.hpp"
#include "Units.hpp"
#include "World/Components/Transform3DComponent.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace N {
static MeshInstance3D& CreatePoint(M::Vector4 col) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& mesh = Primitives::CreateCube("point");
    auto& material = resourceManager.Load<Material>(std::format("m{}{}{}", col.z, col.x, col.y));
    material.Color = col;
    auto& shader = resourceManager.Load<Shader>("pointShader");

    shader.AssignSource(resourceManager.Load<ShaderSource>("pointVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));
    shader.AssignSource(resourceManager.Load<ShaderSource>("pointFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    material.Shader = &shader;

    auto& point = World::Get().CreateEntity<MeshInstance3D>();
    World::Get().Query.Pool<MeshComponent>().GetComponentById(point.Id).Mesh = &mesh;
    World::Get().Query.Pool<MaterialComponent>().GetComponentById(point.Id).Material = &material;
    World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.Id).Scale = { 0.2 };
    World::Get().Root->AttachChild(point);

    return point;
}

static void Plot(const M::Vector3 vec3, const M::Vector4 col = { 1, 1, 1, 1 }) {
    float max = 40;
    if (vec3.x < max && vec3.y < max && vec3.z < max) {
        auto& point = CreatePoint(col);
        auto& transform = World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.Id);
        transform.Position.x = vec3.x;
        transform.Position.y = vec3.y;
        transform.Position.z = vec3.z;
    }
}


static unsigned int cubeId = 0;
static unsigned int cubeId2 = 0;

void Physics::Start() {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& query = World::Get().Query;
    auto& mesh = Primitives::CreateCube("mesh");
    auto& objectShader = resourceManager.Load<Shader>("objectShader");

    objectShader.AssignSource(
        resourceManager.Load<ShaderSource>("objectFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    objectShader.AssignSource(
        resourceManager.Load<ShaderSource>("objectVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));


    auto& objectMaterial = resourceManager.Load<Material>("cubeMaterial");
    objectMaterial.Shader = &objectShader;

    auto& cube = World::Get().CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(cube.Id).Mesh = &mesh;
    query.Pool<MaterialComponent>().GetComponentById(cube.Id).Material = &objectMaterial;
    query.Pool<BodyComponent>().Add(cube.Id);
    cubeId = cube.Id;
    World::Get().Root->AttachChild(cube);

    auto& cube2 = World::Get().CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(cube2.Id).Mesh = &mesh;
    query.Pool<MaterialComponent>().GetComponentById(cube2.Id).Material = &objectMaterial;
    query.Pool<BodyComponent>().Add(cube2.Id).Mass = 10 * Units::Kilogram;
    query.Pool<BodyComponent>().GetComponentById(cube2.Id).Pull = 3;
    cubeId2 = cube2.Id;
    World::Get().Root->AttachChild(cube2);
    query.Pool<Transform3DComponent>().GetComponentById(cubeId2).Position = { 3, 10, 0 };


    // M::Function x = [](const float t) {
    //     return 3 * M::Pow(t, 2) - 2 * t + 3;
    // };
    //
    //
    // U::Logger::Info(x(3 / 8));
    //
    //

    M::Vector2 day1 = M::Vector2::FromPolar({ M::Rad(360 - 45), 25 });
    M::Vector2 day2 = M::Vector2::FromPolar({ M::Rad(60), 40 });

    U::Logger::Info(day1 + day2);
    U::Logger::Info();

    //
    // U::Logger::Info(x.Derivative(2));
    // U::Logger::Info(x.Derivative(2, 1, DifferentiationMethod::Forward, false));
    // U::Logger::Info(x.Differentiate().Derivative(2));
    // U::Logger::Info(x.Differentiate().Derivative(3));
}

static float time = 0;

void Physics::FixedUpdate(double fdt) {
    // auto& resourceManager = Service::Get<ResourceManager>();g
    // auto& input = Engine::Get().GetModule<Input>();
    // auto& query = World::Get().Query;
    // auto& transform = query.Pool<Transform3DComponent>().GetComponentById(cubeId);
    // auto& transform2 = query.Pool<Transform3DComponent>().GetComponentById(cubeId2);
    // auto& body = query.Pool<BodyComponent>().GetComponentById(cubeId);
    // auto& body2 = query.Pool<BodyComponent>().GetComponentById(cubeId2);
    //
    time += fdt;
    //
    // body2.Gravity = (transform.Position - transform2.Position).Normalized() * body.Pull;
    // body2.Force = body2.Gravity * body2.Mass;
    //
    // if (input.IsKeyHeld(Key::Up))
    //     body2.Force.y += 10;
    //
    // if (input.IsKeyHeld(Key::Down))
    //     body2.Force.y -= 10;
    //
    // if (input.IsKeyHeld(Key::Left))
    //     body2.Force.x -= 10;
    //
    // if (input.IsKeyHeld(Key::Right))
    //     body2.Force.x += 10;
    //
    //
    // M::Vector3 acceleration = body2.Force / body2.Mass;
    // body2.Velocity += acceleration * fdt;
    // transform2.Position += body2.Velocity * fdt;
    //
    //
    // body.Gravity = (transform2.Position - transform.Position).Normalized() * body2.Pull;
    // body.Force = body.Gravity * body.Mass;
    // //
    // // if (input.IsKeyHeld(Key::Up))
    // //     body.Force.y += 10;
    // //
    // // if (input.IsKeyHeld(Key::Down))
    // //     body.Force.y -= 10;
    // //
    // // if (input.IsKeyHeld(Key::Left))
    // //     body.Force.x -= 10;
    // //
    // // if (input.IsKeyHeld(Key::Right))
    // //     body.Force.x += 10;
    //
    //
    // M::Vector3 acceleration2 = body.Force / body.Mass;
    // body.Velocity += acceleration2 * fdt;
    // transform.Position += body.Velocity * fdt;

    M::Function f = [](const float t) { return 2 + 3 * t - 4 * M::Pow(t, 2); };

    Plot({ time, f(time), 0 }, M::Color::Blue);
    Plot({ time, f.Derivative(time), 0 }, M::Color::Red);
    // U::Logger::Info(f.Differentiate().Derivative(time));
}
} // namespace N
