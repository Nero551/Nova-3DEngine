#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace N {
struct Transform3DSystem : System {
    void FixedUpdate(double fdt) override;
};
} // namespace N
