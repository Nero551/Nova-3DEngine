#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace N
{
struct Transform3DSystem : C::System
{
    void Update(double fdt) override;
};
} // namespace N
