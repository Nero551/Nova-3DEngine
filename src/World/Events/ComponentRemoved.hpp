#pragma once
#include "Core/OuterCore/Event.hpp"
namespace N
{
struct ComponentRemoved : Event<ComponentRemoved>
{
    unsigned int EntityId;

    ComponentRemoved(unsigned int entityId) : EntityId(entityId) {}
};
} // namespace N