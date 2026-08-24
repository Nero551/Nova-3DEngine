#pragma once
#include "Math/Vector/Vector3.hpp"

namespace E::M {
struct QPolar {
    Vector3 Axis;
    float Angle;
    float Magnitude;

    QPolar(Vector3 axis, float angle, float magnitude = 1) : Axis(axis.Normalized()), Angle(angle), Magnitude(magnitude) {
    }
};
} // namespace E::M
