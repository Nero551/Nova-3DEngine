#pragma once
#include "Core/OuterCore/ECS/System.hpp"

namespace N {
struct System : C::System {
    void Start() override;
};
}