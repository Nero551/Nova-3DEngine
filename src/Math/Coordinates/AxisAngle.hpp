#pragma once
#include "Math/Vector/Vector3.hpp"

namespace E::M {
struct AxisAngle {
    Vector3 Axis;
    float Angle;

    AxisAngle(Vector3 axis, float angle) : Axis(axis), Angle(angle) {
    }
};
} // namespace E::M
