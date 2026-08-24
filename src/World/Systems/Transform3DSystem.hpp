#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace E {
struct Transform3DSystem : System {
    void Update(double dt) override;
};
} // namespace E
