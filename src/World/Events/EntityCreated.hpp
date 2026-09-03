#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/OuterCore/Event.hpp"

namespace N {
struct EntityCreated : Event<EntityCreated> {
    Entity& entity;

    EntityCreated(Entity& entity) : entity(entity) {
    }
};
} // namespace N
