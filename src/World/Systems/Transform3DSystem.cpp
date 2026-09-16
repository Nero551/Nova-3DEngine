#include "Transform3DSystem.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N
{
void Transform3DSystem::Update(double fdt)
{
    auto& world = World::Get();
    auto& query = world.Query;
    auto& transformPool = query.Pool<Transform3DComponent>();

    world.GetRoot().ForEachDescendant(
        [&](unsigned int entityId)
        {
            auto& entity = world.FindEntity(entityId);

            if (!transformPool.HasId(entityId))
            {
                return;
            }

            auto& transform = transformPool.GetComponentById(entityId);

            if (transform.InheritTransform)
            {
                auto& parent = entity.GetParent();

                if (transformPool.HasId(parent.GetId()))
                {
                    auto& parentTransform = transformPool.GetComponentById(parent.GetId());

                    transform.GlobalPosition = parentTransform.GlobalPosition + transform.Position;
                    transform.GlobalRotation = parentTransform.GlobalRotation * transform.Rotation;
                    transform.GlobalScale = parentTransform.GlobalScale * transform.Scale;

                    return;
                }
            }
            transform.GlobalPosition = transform.Position;
            transform.GlobalRotation = transform.Rotation;
            transform.GlobalScale = transform.Scale;
        });
}
} // namespace N
