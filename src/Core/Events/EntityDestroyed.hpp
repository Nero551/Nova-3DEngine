#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/OuterCore/Event.hpp"

namespace N::C
{
struct EntityDestroyed : Event<EntityDestroyed>
{
    C::Entity& entity;

    EntityDestroyed(C::Entity& entity) : entity(entity) {}
};
} // namespace N::C
