#pragma once

#include "Core/OuterCore/ECS/Component.hpp"
#include "Math/Vector/Vector3.hpp"

namespace N {
struct BodyComponent : Component {
    M::Vector3 Velocity = { 0, 0, 0 };
    M::Vector3 Force = M::Vector3::Zero;
    float Mass = Units::Kilogram;
    float Pull = 9.8;
    M::Vector3 Gravity;
};
} // namespace N
