#pragma once
#include "Core/OuterCore/ECS/System.hpp"

namespace N {
struct LightingSystem : System {
    void Start() override;
    void Render() override;
};
} // namespace N
