#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/OuterCore/Event.hpp"

namespace N
{
struct EntityCreated : C::Event<EntityCreated>
{
    C::Entity& entity;

    EntityCreated(C::Entity& entity) : entity(entity) {}
};
} // namespace N
