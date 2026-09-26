#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/OuterCore/Event.hpp"

namespace N::C
{
struct EntityCreated : Event<EntityCreated>
{
    C::Entity& entity;

    EntityCreated(C::Entity& entity) : entity(entity) {}
};
} // namespace N::C
