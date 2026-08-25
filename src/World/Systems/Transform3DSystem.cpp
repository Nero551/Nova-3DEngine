#include "Transform3DSystem.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
void Transform3DSystem::FixedUpdate(double fdt) {
    for (auto& entity : World::Get().Root->GetDescendants()) {
        if (!entity->HasComponent<Transform3DComponent>()) {
            continue;
        }

        auto& transform = entity->GetComponent<Transform3DComponent>();

        if (entity->HasParent() && transform.InheritTransform) {
            auto& parent = entity->GetParent();
            if (parent.HasComponent<Transform3DComponent>()) {
                auto& parentTransform = parent.GetComponent<Transform3DComponent>();

                transform.GlobalPosition = parentTransform.GlobalPosition + transform.Position;
                transform.GlobalRotation = parentTransform.GlobalRotation * transform.Rotation;
                transform.GlobalScale = parentTransform.GlobalScale * transform.Scale;
                continue;
            }
        }
        transform.GlobalPosition = transform.Position;
        transform.GlobalRotation = transform.Rotation;
        transform.GlobalScale = transform.Scale;
    }
}
} // namespace N
