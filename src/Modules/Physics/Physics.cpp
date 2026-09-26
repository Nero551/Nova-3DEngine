#include "Physics.hpp"

#include "Components/BodyComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Complex/Complex.hpp"
#include "Modules/Graphics/Components/MaterialComponent.hpp"
#include "Modules/Graphics/Components/MeshComponent.hpp"
#include "Modules/Graphics/Primitives/Primitives.hpp"
#include "Modules/Input/Input.hpp"
#include "World/Components/Transform3DComponent.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace N
{
static C::Entity& CreatePoint(M::Vector4 col)
{
    auto& resourceManager = C::Service::Get<C::ResourceManager>();
    auto& mesh = Primitives::CreateCube("point");
    auto& material = resourceManager.Load<Material>(std::format("m{}{}{}", col.z, col.x, col.y));
    material.Color = col;
    auto& shader = resourceManager.Load<Shader>("pointShader");

    shader.AssignSource(
        resourceManager.Load<ShaderSource>("pointVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));
    shader.AssignSource(
        resourceManager.Load<ShaderSource>("pointFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    material.Shader = &shader;

    auto& point = C::World::Get().CreateEntity<MeshInstance3D>();
    C::World::Get().Query.Pool<MeshComponent>().GetComponentById(point.GetId()).Mesh = &mesh;
    C::World::Get().Query.Pool<MaterialComponent>().GetComponentById(point.GetId()).Material = &material;
    C::World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.GetId()).Scale =
        M::Vector3{0.2};
    C::World::Get().GetRoot().AttachChild(point);

    return point;
}

static void Plot(const M::Vector3 vec3, const M::Vector4 col = {1, 1, 1, 1})
{
    float max = 40;
    if (vec3.x < max && vec3.y < max && vec3.z < max)
    {
        auto& point = CreatePoint(col);
        auto& transform = C::World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.GetId());
        transform.Position().x = vec3.x;
        transform.Position().y = vec3.y;
        transform.Position().z = vec3.z;
    }
}

static unsigned int cubeId = 0;

void Physics::Start()
{
    auto& resourceManager = C::Service::Get<C::ResourceManager>();
    auto& query = C::World::Get().Query;
    auto& mesh = Primitives::CreateCube("mesh");
    auto& objectShader = resourceManager.Load<Shader>("objectShader");

    objectShader.AssignSource(resourceManager.Load<ShaderSource>(
        "objectFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    objectShader.AssignSource(
        resourceManager.Load<ShaderSource>("objectVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));

    auto& objectMaterial = resourceManager.Load<Material>("cubeMaterial");
    objectMaterial.Shader = &objectShader;

    auto& cube = C::World::Get().CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(cube.GetId()).Mesh = &mesh;
    query.Pool<MaterialComponent>().GetComponentById(cube.GetId()).Material = &objectMaterial;
    query.Pool<BodyComponent>().Add(cube.GetId());
    cubeId = cube.GetId();
    C::World::Get().GetRoot().AttachChild(cube);
}

static float time = 0;
static M::Vector3 ExternalForces;

void Physics::FixedUpdate(const double fdt)
{
    time += fdt / 2;

    float g = -9.8;

    auto& resourceManager = C::Service::Get<C::ResourceManager>();
    auto& input = C::Engine::Get().GetModule<Input>();
    auto& query = C::World::Get().Query;
    auto& transform = query.Pool<Transform3DComponent>().GetComponentById(cubeId);
    auto& body = query.Pool<BodyComponent>().GetComponentById(cubeId);

    // ExternalForces = M::Vector3{0};
    // M::Vector3 friction = {-body.Velocity.x * M::PHI, 0, -body.Velocity.z * M::PHI};
    //
    // if (input.IsKeyPressed(Key::E))
    // {
    //     body.Velocity = {15, 10, 0};
    // }
    //
    // if (input.IsKeyHeld(Key::Up))
    //     ExternalForces.y += 50;
    //
    // if (input.IsKeyHeld(Key::Down))
    //     ExternalForces.y -= 50;
    //
    // if (input.IsKeyHeld(Key::Left))
    //     ExternalForces.x -= 50;
    //
    // if (input.IsKeyHeld(Key::Right))
    //     ExternalForces.x += 50;
    //
    // body.Force = M::Vector3{0, -9.8, 0} + ExternalForces + friction;
    //
    // M::Vector3 acceleration = body.Force / body.Mass;
    // body.Velocity += acceleration * fdt;
    // transform.Position += body.Velocity * fdt;
    // transform.Position.y = std::max(transform.Position.y, 0.0f);
}
} // namespace N
