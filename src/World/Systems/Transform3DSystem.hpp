#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace N {
struct Transform3DSystem : System {
    void Update(double dt) override;
};
} // namespace N
