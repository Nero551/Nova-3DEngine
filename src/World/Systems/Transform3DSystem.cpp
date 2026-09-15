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

    for (auto& entity : world.GetRoot().GetDescendants())
    {
        if (!transformPool.HasId(entity->Id))
        {
            continue;
        }

        auto& transform = transformPool.GetComponentById(entity->Id);

        if (transform.InheritTransform)
        {
            auto& parent = entity->GetParent();

            if (transformPool.HasId(parent.Id))
            {
                auto& parentTransform = transformPool.GetComponentById(parent.Id);

                transform.GlobalPosition = parentTransform.GlobalPosition + transform.Position;
                transform.GlobalRotation = parentTransform.GlobalRotation * transform.Rotation;
                transform.GlobalScale = parentTransform.GlobalScale * transform.Scale;

                return;
            }
        }
        transform.GlobalPosition = transform.Position;
        transform.GlobalRotation = transform.Rotation;
        transform.GlobalScale = transform.Scale;
    }
}
} // namespace N
