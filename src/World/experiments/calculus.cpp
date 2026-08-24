#include "calculus.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Complex/Complex.hpp"
#include "Math/Complex/Constants.hpp"
#include "Math/Quaternion/Exponentials.hpp"
#include "Math/Quaternion/Quaternion.hpp"
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

static std::vector<U::CheckedPtr<Entity>> points = {};

static Entity& Plot(const M::Vector3 vec3, const M::Vector4 col = { 1, 1, 1, 1 }) {
    // if (vec3.y < max && vec3.y > min) {
    auto& point = CreatePoint(col);
    auto& transform = point.GetComponent<Transform3DComponent>();
    transform.Position.x = vec3.x;
    transform.Position.y = vec3.y;
    transform.Position.z = vec3.z;

    return point;
    // }
}

static constexpr float step = 0.025;
static constexpr float xRange = 10;
static float x = -5;

void calculus::Start() {
    // M::Vector4 v4 = M::Vector4::FromHyperSpherical({ M::Rad(45), M::Rad(36), M::Rad(59) });
    // U::Logger::Info(M::Deg(v4.Elevation()));
    // U::Logger::Info(M::Deg(v4.Azimuth()));
    // U::Logger::Info(M::Deg(v4.HyperAngle()));

    float increase = 30;

    float theta = M::Rad(32);

    M::Quaternion q = M::Quaternion::FromEulerXYZ({ M::Rad(90), 0, 0 });
    U::Logger::Info(M::Deg(q.ToEulerXYZ().x));
    M::Vector3 v = { 0, 0, -1 };
    v = q.Transform(v);
    Plot(v);
}

static float elapsed = 0;
static float passed = 0;

static float multiplier = 1;

void calculus::Update(double dt) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& input = Engine::Get().GetModule<Input>();


    if (input.IsKeyHeld(Key::Up)) {
        multiplier += 0.1;
    }
    if (input.IsKeyHeld(Key::Down)) {
        multiplier -= 0.1;
    }

    for (auto& point : points) {
        auto& transform = point->GetComponent<Transform3DComponent>();
        transform.Position *= multiplier;
    }

    multiplier = 1;
}

void calculus::TwoDimensionalProjection(float increase) {
    for (int theta = -180; theta < 180; theta += increase) {
        M::Vector2 v2 = M::Vector2::FromPolar(M::Polar(theta));
        float proj = v2.StereoProject();
        auto& d2point = Plot({ v2.x, v2.y, 0 });
        auto& point = Plot({ proj, 0, 0 });
        points.emplace_back(&point);
        points.emplace_back(&d2point);
    }
}

void calculus::ThreeDimensionalProjection(float increase) {
    for (int theta = -180; theta < 180; theta += increase) {
        for (int phi = -180; phi < 180; phi += increase) {
            M::Vector3 v3 = M::Vector3::FromSpherical(M::Spherical(theta, phi));
            auto& d3point = Plot(v3);
            M::Vector2 proj = v3.StereoProject();
            auto& point = Plot({ proj.x, proj.y, 0 });
            points.emplace_back(&point);
            points.emplace_back(&d3point);
        }
    }
}

void calculus::FourDimensionalProjection(float increase) {
    for (int theta = -180; theta < 180; theta += increase) {
        for (int phi = -180; phi < 180; phi += increase) {
            for (int h = -180; h < 180; h += increase) {
                M::Vector4 v4 = M::Vector4::FromHyperSpherical(M::HyperSpherical(theta, phi, h));
                M::Vector3 proj = v4.StereoProject();
                auto& point = Plot(proj);
                points.emplace_back(&point);
            };
        }
    }
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
