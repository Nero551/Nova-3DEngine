#include "Physics.hpp"

#include "Components/BodyComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"
#include "Math/Complex/Complex.hpp"
#include "Math/Functions/Function.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Components/MaterialComponent.hpp"
#include "Modules/Renderer/Components/MeshComponent.hpp"
#include "Modules/Renderer/Primitives/Primitives.hpp"
#include "World/Components/Transform3DComponent.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace N
{
static MeshInstance3D& CreatePoint(M::Vector4 col)
{
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& mesh = Primitives::CreateCube("point");
    auto& material = resourceManager.Load<Material>(std::format("m{}{}{}", col.z, col.x, col.y));
    material.Color = col;
    auto& shader = resourceManager.Load<Shader>("pointShader");

    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "pointVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));
    shader.AssignSource(resourceManager.Load<ShaderSource>(
        "pointFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    material.Shader = &shader;

    auto& point = World::Get().CreateEntity<MeshInstance3D>();
    World::Get().Query.Pool<MeshComponent>().GetComponentById(point.Id).Mesh = &mesh;
    World::Get().Query.Pool<MaterialComponent>().GetComponentById(point.Id).Material = &material;
    World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.Id).Scale =
        M::Vector3{0.2};
    World::Get().Root->AttachChild(point);

    return point;
}

static void Plot(const M::Vector3 vec3, const M::Vector4 col = {1, 1, 1, 1})
{
    float max = 40;
    if (vec3.x < max && vec3.y < max && vec3.z < max)
    {
        auto& point = CreatePoint(col);
        auto& transform =
            World::Get().Query.Pool<Transform3DComponent>().GetComponentById(point.Id);
        transform.Position.x = vec3.x;
        transform.Position.y = vec3.y;
        transform.Position.z = vec3.z;
    }
}

static unsigned int cubeId = 0;

void Physics::Start()
{
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& query = World::Get().Query;
    auto& mesh = Primitives::CreateCube("mesh");
    auto& objectShader = resourceManager.Load<Shader>("objectShader");

    objectShader.AssignSource(resourceManager.Load<ShaderSource>(
        "objectFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    objectShader.AssignSource(resourceManager.Load<ShaderSource>(
        "objectVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));

    auto& objectMaterial = resourceManager.Load<Material>("cubeMaterial");
    objectMaterial.Shader = &objectShader;

    auto& cube = World::Get().CreateEntity<MeshInstance3D>();
    query.Pool<MeshComponent>().GetComponentById(cube.Id).Mesh = &mesh;
    query.Pool<MaterialComponent>().GetComponentById(cube.Id).Material = &objectMaterial;
    query.Pool<BodyComponent>().Add(cube.Id);
    cubeId = cube.Id;
    World::Get().Root->AttachChild(cube);

    M::Function<M::Vector2, M::Vector2> ProjectilePosition = [](const M::Vector2 vi)
    {
        M::Vector2 a = {0, -9.8};
        float t = -vi.y / a.y;
        M::Vector2 vf = {vi.x + a.x * t, 0};
        M::Vector2 p = (vf + vi) * t / 2.0;
        p.x *= 2;
        return p;
    };

    M::Vector2 vi = M::Vector2::FromPolar({M::Rad(20), 11});
    U::Logger::Info(ProjectilePosition(vi));
}

static float time = 0;
static M::Vector3 ExternalForces;

void Physics::FixedUpdate(double fdt)
{
    time += fdt;

    M::Vector2 a = {0, -9.8};
    M::Vector2 vi = {5, 5};
    M::Vector2 pi = {0, 0};

    M::Function<float, M::Vector2> v = [vi, a](const auto t)
    {
        M::Vector2 vf = vi + a * t;
        return vf;
    };

    M::Function<float, M::Vector2> p = v.Integrate(0);

    Plot({p(time).x, p(time).y, 0}, M::Color::Blue);
    Plot({v(time).x, v(time).y, 0}, M::Color::Red);
    Plot({v.Derivative(time).x, v.Derivative(time).y, 0}, M::Color::Green);

    if (M::NearlyEquals(v(time).Angle(), v.Derivative(time).Angle()))
    {
        U::Logger::Info(v(time).Angle(), " ", v.Derivative(time).Angle());
    }

    auto& resourceManager = Service::Get<ResourceManager>();
    auto& input = Engine::Get().GetModule<Input>();
    auto& query = World::Get().Query;
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
