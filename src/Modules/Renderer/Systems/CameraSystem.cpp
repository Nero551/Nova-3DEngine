#include "CameraSystem.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Modules/Input/Input.hpp"
#include "Modules/Renderer/Components/CameraComponent.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
void CameraSystem::FixedUpdate(const double fdt) {
    {
        auto& input = Engine::Get().GetModule<Input>();
        auto& camera = World::Get().ActiveCamera;
        auto& transform = camera->GetComponent<Transform3DComponent>();
        auto& cameraComponent = camera->GetComponent<CameraComponent>();

        cameraComponent.AspectRatio = Engine::Get().Window.GetAspectRatio();

        if (input.GetMouseMode() == MouseMode::Disabled) {
            cameraComponent.Speed += input.GetScrollDelta().y / 3;
            cameraComponent.Speed = std::clamp(cameraComponent.Speed, 1.0f, 50.0f);

            const float xOffset = M::Rad(input.GetMouseDelta().x * cameraComponent.Sensitivity);
            const float yOffset = M::Rad(-input.GetMouseDelta().y * cameraComponent.Sensitivity);

            const float maxPitch = M::Rad(89.9);

            cameraComponent.Yaw += xOffset;
            cameraComponent.Pitch += yOffset;
            cameraComponent.Pitch = std::clamp(cameraComponent.Pitch, -maxPitch, maxPitch);

            transform.Rotation = M::Quaternion::FromEulerXYZ({ cameraComponent.Pitch, cameraComponent.Yaw, 0 });
        }

        const float speed = cameraComponent.Speed * static_cast<float>(fdt);

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
}

M::Matrix4 CameraSystem::GetViewMatrix() {
    auto& camera = World::Get().ActiveCamera;
    auto& transformComponent = camera->GetComponent<Transform3DComponent>();

    M::Vector3 pos = transformComponent.Position;
    M::Vector3 forward = transformComponent.GetForward();
    M::Vector3 up = transformComponent.GetUp();

    M::Matrix4 view = M::Matrix4::LookAt(pos, pos + forward, up);

    return view;
}
} // namespace N
