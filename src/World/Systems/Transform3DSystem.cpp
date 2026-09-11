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

    query.ForEach<Transform3DComponent>(
        [&](unsigned int entityId, Transform3DComponent& transform)
        {
            auto& entity = world.FindEntity(entityId);

            if (transform.InheritTransform)
            {
                auto& parent = entity.GetParent();

                if (transformPool.HasId(parent.Id))
                {
                    auto& parentTransform = transformPool.GetComponentById(parent.Id);

                    if (transform.Position.IsChanged() || parentTransform.GlobalPosition.IsChanged())
                    {
                        transform.GlobalPosition = parentTransform.GlobalPosition + transform.Position;
                    }

                    if (transform.Rotation.IsChanged() || parentTransform.GlobalRotation.IsChanged())
                    {
                        transform.GlobalRotation = parentTransform.GlobalRotation * transform.Rotation;
                    }

                    if (transform.Scale.IsChanged() || parentTransform.GlobalScale.IsChanged())
                    {
                        transform.GlobalScale = parentTransform.GlobalScale * transform.Scale;
                    }

                    return;
                }
            }

            if (transform.Position.IsChanged())
            {
                transform.GlobalPosition = transform.Position;
            }

            if (transform.Rotation.IsChanged())
            {
                transform.GlobalRotation = transform.Rotation;
            }

            if (transform.Scale.IsChanged())
            {
                transform.GlobalScale = transform.Scale;
            }
        });
}
} // namespace N
