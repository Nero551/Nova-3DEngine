#include "CameraSystem.hpp"

#include <cassert>

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Components/CameraComponent.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
void CameraSystem::Update(const double dt) {
    auto& world = World::Get();
    auto& query = world.Query;
    auto& input = Engine::Get().GetModule<Input>();
    auto& camera = world.ActiveCamera;

    auto& transform = query.Pool<Transform3DComponent>().GetComponentById(camera->Id);
    auto& cameraComponent = query.Pool<CameraComponent>().GetComponentById(camera->Id);

    cameraComponent.AspectRatio = Engine::Get().Window.GetAspectRatio();

    if (input.GetMouseMode() == MouseMode::Disabled) {
        cameraComponent.Speed += input.GetScrollDelta().y / 3;
        cameraComponent.Speed = std::clamp(cameraComponent.Speed, 1.0f, 50.0f);

        const float xOffset = M::Rad(-input.GetMouseDelta().x * cameraComponent.Sensitivity);
        const float yOffset = M::Rad(-input.GetMouseDelta().y * cameraComponent.Sensitivity);

        const float maxPitch = M::Rad(89.9);

        cameraComponent.Yaw += xOffset;
        cameraComponent.Pitch += yOffset;
        cameraComponent.Pitch = std::clamp(cameraComponent.Pitch, -maxPitch, maxPitch);

        transform.Rotation = M::Quaternion::FromEulerXYZ({ cameraComponent.Pitch, cameraComponent.Yaw, 0 });
    }

    U::Logger::Info(transform.GlobalRotation);

    const float speed = cameraComponent.Speed * static_cast<float>(dt);

    if (input.IsKeyHeld(Key::W)) {
        transform.Position += speed * transform.GetForward();
    }

    if (input.IsKeyHeld(Key::S)) {
        transform.Position -= speed * transform.GetForward();
    }

    if (input.IsKeyHeld(Key::A)) {
        transform.Position -= speed * transform.GetRight();
    }

    if (input.IsKeyHeld(Key::D)) {
        transform.Position += speed * transform.GetRight();
    }

    if (input.IsKeyHeld(Key::Space)) {
        transform.Position += speed * M::Vector3(0, 1, 0);
    }

    if (input.IsKeyHeld(Key::LeftShift)) {
        transform.Position -= speed * M::Vector3(0, 1, 0);
    }
}

M::Matrix4 CameraSystem::GetViewMatrix() {
    auto& world = World::Get();
    auto& camera = world.ActiveCamera;
    auto& transform = world.Query.Pool<Transform3DComponent>().GetComponentById(camera->Id);

    const M::Vector3 pos = transform.Position;
    const M::Vector3 forward = transform.GetForward();
    const M::Vector3 up = transform.GetUp();

    return M::Matrix4::LookAt(pos, pos + forward, up);
}
} // namespace N
