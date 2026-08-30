#include "calculus.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/InnerCore/World.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"
#include "Math/Functions/Function.hpp"
#include "Math/Quaternion/Quaternion.hpp"
#include "Math/Vector/Vector4.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Primitives/Primitives.hpp"
#include "Modules/Renderer/Resources/Texture/Cubemap.hpp"
#include "Modules/Renderer/Resources/Texture/Texture2D.hpp"
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

static unsigned int cubeId = 0;

void calculus::Start() {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& mesh = Primitives::CreateCube("mesh");
    auto& objectShader = resourceManager.Load<Shader>("objectShader");

    objectShader.AssignSource(
        resourceManager.Load<ShaderSource>("objectFrag", "Assets/Shaders/shader.frag", ShaderStage::Fragment));
    objectShader.AssignSource(
        resourceManager.Load<ShaderSource>("objectVert", "Assets/Shaders/shader.vert", ShaderStage::Vertex));


    auto& objectMaterial = resourceManager.Load<Material>("cubeMaterial");
    objectMaterial.Shader = &objectShader;

    auto& cube = World::Get().CreateEntity<MeshInstance3D>();
    cube.GetComponent<MeshComponent>().Mesh = &mesh;
    cube.GetComponent<MaterialComponent>().Material = &objectMaterial;
    cube.GetComponent<Transform3DComponent>().Scale = { 1 };
    auto& transform = cube.GetComponent<Transform3DComponent>();
    cubeId = cube.Id;
    World::Get().Root->AttachChild(cube);

    ThreeDimensionalProjection(10);
}

static float elapsed = 0;
static float passed = 0;

static float multiplier = 1;

void calculus::Update(double dt) {
    auto& resourceManager = Service::Get<ResourceManager>();
    auto& transform = World::Get().FindEntity(cubeId).GetComponent<Transform3DComponent>();
    auto& input = Engine::Get().GetModule<Input>();

    // x += step;
    // if (x >= xRange) {
    //     return;
    // }


    if (input.IsKeyHeld(Key::Z)) {
        transform.Rotation *= M::Quaternion::FromEulerXYZ({ 0.1, 0, 0 });
    }

    if (input.IsKeyHeld(Key::X)) {
        transform.Rotation *= M::Quaternion::FromEulerXYZ({ 0, 0.1, 0 });
    }

    if (input.IsKeyHeld(Key::C)) {
        transform.Rotation *= M::Quaternion::FromEulerXYZ({ 0, 0, 0.1 });
    }

    if (input.IsKeyHeld(Key::Left)) {
        multiplier -= 0.1;
    }
    if (input.IsKeyHeld(Key::Right)) {
        multiplier += 0.1;
    }

    for (auto& point : points) {
        auto& transform = point->GetComponent<Transform3DComponent>();
        transform.Position *= multiplier;
    }

    multiplier = 1;

    // M::Function sin = [](const float x) {
    //     return std::sin(x);
    // };
    //
    // U::Logger::Info(sin.Taylor(3, 0)(x));
    // Plot({x, sin.Maclaurin(4)(x), 0});
    // Plot({x, sin(x), 0}, M::Color::Red);
}

void calculus::TwoDimensionalProjection(float increase) {
    for (int theta = -180; theta < 180; theta += increase) {
        M::Vector2 v2 = M::Vector2::FromPolar(M::Polar(theta));
        float proj = v2.StereoProject();
        // auto& d2point = Plot({v2.x, v2.y, 0});
        auto& point = Plot({ proj, 0, 0 });
        points.emplace_back(&point);
        // points.emplace_back(&d2point);
    }
}

void calculus::ThreeDimensionalProjection(float increase) {
    for (int theta = -180; theta < 180; theta += increase) {
        for (int phi = -180; phi < 180; phi += increase) {
            M::Vector3 v3 = M::Vector3::FromSpherical(M::Spherical(theta, phi));
            // auto& d3point = Plot(v3);
            M::Vector2 proj = v3.StereoProject();
            auto& point = Plot({ proj.x, proj.y, 0 });
            points.emplace_back(&point);
            // points.emplace_back(&d3point);
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
} // namespace N

//
// M::Vector3 Tween(M::Vector3 start, M::Vector3 end, float duration, double dt)
// {
//     passed += dt;
//     float t = M::Clamp(passed / duration, 0, 1);
//     M::Vector3 current = start.Lerp(end, M::EaseOutQuad(t));
//     return current;
// }
