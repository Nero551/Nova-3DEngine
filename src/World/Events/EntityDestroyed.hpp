#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/OuterCore/Event.hpp"

namespace N {
struct EntityDestroyed : Event<EntityDestroyed> {
    Entity& entity;

    EntityDestroyed(Entity& entity) : entity(entity) {
    }
};
} // namespace N
