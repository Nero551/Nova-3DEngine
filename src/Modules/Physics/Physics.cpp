#include "Physics.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"
#include "Math/Common/Constraints.hpp"
#include "Math/Common/Exponentials.hpp"
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
    cubeId = cube.Id;
    World::Get().Root->AttachChild(cube);

    M::Function v = [](const float t) { return 40 - 5 * M::Pow(t, 2); };

    U::Logger::Info();
}

static M::Vector3 position = { 0, 10, 0 };
static M::Vector3 velocity = { 0, 0, 0 };
static M::Vector3 acceleration = { 0, 0, 0 };
static M::Vector3 force = { 0, 0, 0 };
static float mass = Units::Kilogram;

static float time = 0;

static const M::Vector3 Gravity = { 0, -9.8f, 0 };

void Physics::FixedUpdate(double fdt) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& input = Engine::Get().GetModule<Input>();
    auto& query = World::Get().Query;
    auto& cube = World::Get().FindEntity(cubeId);
    auto& transform = query.Pool<Transform3DComponent>().GetComponentById(cube.Id);

    time += fdt;

    force = Gravity * mass;

    if (input.IsKeyHeld(Key::Up))
        force.y += 1.5f;

    if (input.IsKeyHeld(Key::Down))
        force.y -= 1.5f;

    if (input.IsKeyHeld(Key::Left))
        force.x -= 1.5f;

    if (input.IsKeyHeld(Key::Right))
        force.x += 1.5f;


    acceleration = force / mass;
    velocity += acceleration * fdt;
    position += velocity * fdt;

    transform.Position = position;
    transform.Position.y = std::max(transform.Position.y, 0.0f);

    // U::Logger::Info(position, velocity, acceleration, force);

    Plot({ time, position.x, 0 }, M::Color::White);
    Plot({ time, velocity.x, 0 }, M::Color::Blue);
    Plot({ time, acceleration.x, 0 }, M::Color::Green);
    // Plot({time, force.x, 0}, M::Color::Red);
}
} // namespace N
