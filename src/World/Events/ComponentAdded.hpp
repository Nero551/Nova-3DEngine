#pragma once
#include "Core/OuterCore/Event.hpp"

namespace N
{
struct ComponentAdded : Event<ComponentAdded>
{
    unsigned int EntityId;

    ComponentAdded(const unsigned int entityId) : EntityId(entityId) {}
};
} // namespace N
