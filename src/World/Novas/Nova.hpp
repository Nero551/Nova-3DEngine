#pragma once
#include "Core/OuterCore/ECS/Entity.hpp"

namespace N {
struct Nova : Entity {
    void Initialize() override {
        Entity::Initialize();
    }
};
} // namespace N
