#include "Transform3DSystem.hpp"

#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N
{
void Transform3DSystem::Update(double fdt)
{
    auto &query = World::Get().Query;
    for (auto [entityId, transform] : query.With<Transform3DComponent>())
    {
        auto &entity = World::Get().FindEntity(entityId);

        if (entity.HasParent() && transform.InheritTransform)
        {
            auto &parent = entity.GetParent();
            if (query.Pool<Transform3DComponent>().HasId(parent.Id))
            {
                auto &parentTransform =
                    query.Pool<Transform3DComponent>().GetComponentById(parent.Id);

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
