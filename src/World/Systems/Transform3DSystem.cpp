#include "Transform3DSystem.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
void Transform3DSystem::Update(double fdt) {
    auto& world = World::Get();
    for (auto [entityId, transform] : World::Get().Query.With<Transform3DComponent>()) {
        auto& entity = World::Get().FindEntity(entityId);

        if (entity.HasParent() && transform.InheritTransform) {
            auto& parent = entity.GetParent();
            if (World::Get().Query.Pool<Transform3DComponent>().HasId(parent.Id)) {
                auto& parentTransform = World::Get().Query.Pool<Transform3DComponent>().GetComponentById(parent.Id);

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
