#include "calculus.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Complex/Complex.hpp"
#include "Math/Complex/Constants.hpp"
#include "Math/Vector/Vector4.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Primitives/Primitives.hpp"
#include "World/Novas/Light.hpp"
#include "World/Novas/MeshInstance3D.hpp"

namespace E {
static MeshInstance3D& CreatePoint(M::Vector4 col) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& mesh = Primitives::CreateCube("point");
    auto& material = resourceManager.Load<Material>(std::format("m{}{}{}", col.z, col.x, col.y));
    material.Color = col;
    auto& shader = resourceManager.Load<Shader>("shader");

    shader.AssignSource(resourceManager.Load<ShaderSource>("pointVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));
    shader.AssignSource(resourceManager.Load<ShaderSource>("pointFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    material.Shader = &shader;

    mesh.RenderMode = RenderMode::SolidWireframe;

    auto& point = World::Get().CreateEntity<MeshInstance3D>();
    point.GetComponent<MeshComponent>().Mesh = &mesh;
    point.GetComponent<MaterialComponent>().Material = &material;
    point.GetComponent<Transform3DComponent>().Scale = { 0.2 };
    World::Get().Root->AttachChild(point);

    return point;
}

static float max = 100;
static float min = -100;

static std::vector<U::CheckedPtr<MeshInstance3D>> points = {};

static void Plot(const M::Vector3 vec3, const M::Vector4 col = { 1, 1, 1, 1 }) {
    // if (vec3.y < max && vec3.y > min) {
    auto& point = CreatePoint(col);
    auto& transform = point.GetComponent<Transform3DComponent>();
    transform.Position.x = vec3.x;
    transform.Position.y = vec3.y;
    transform.Position.z = vec3.z;
    // }
}

static constexpr float step = 0.025;
static constexpr float xRange = 10;
static float x = -5;

void calculus::Start() {
    // for (float x = -8; x < 8; x += 0.05) {
    //     M::Complex a = (std::cos(x) + M::I * std::sin(x));
    //     Plot({ a.Real, a.Imaginary, x });
    //     points.emplace_back(&point);
    // }

    E::M::Vector3 v3 = E::M::Vector3::FromSpherical({ M::Rad(68), M::Rad(37) });
    E::U::Logger::Info(M::Deg(v3.Azimuth()));
    E::U::Logger::Info(M::Deg(v3.Elevation()));

    Plot(v3);
}

static float elapsed = 0;
static float passed = 0;

static float r = 1;
static float period = 1;

void calculus::Update(double dt) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& input = Engine::Get().GetModule<Input>();


    if (input.IsKeyHeld(Key::Up)) {
        r += 0.1;
    }
    if (input.IsKeyHeld(Key::Down)) {
        r -= 0.1;
    }
    if (input.IsKeyHeld(Key::Right)) {
        period += 0.1;
    }
    if (input.IsKeyHeld(Key::Left)) {
        period -= 0.1;
    }
    //
    // for (auto& point : points) {
    //     auto& transform = point->GetComponent<Transform3DComponent>();
    //     M::Complex a = r * (std::cos(transform.Position.z * period) + M::I * std::sin(transform.Position.z * period));
    //     transform.Position.x = a.Real;
    //     transform.Position.y = a.Imaginary;
    // }
}
} // namespace E

//
// M::Vector3 Tween(M::Vector3 start, M::Vector3 end, float duration, double dt)
// {
//     passed += dt;
//     float t = M::Clamp(passed / duration, 0, 1);
//     M::Vector3 current = start.Lerp(end, M::EaseOutQuad(t));
//     return current;
// }
