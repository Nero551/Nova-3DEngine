#pragma once
#include "Math/Vector/Vector3.hpp"

namespace E::M {

/**
 * @brief Represents a quaternion in polar form.
 * A quaternion can be represented as `q = m(cos(θ) + u sin(θ))`,
 * where `m` is the magnitude, `θ` is the angle, and `u` is the normalized imaginary-axis direction.
 */
struct QPolar {
    Vector3 Axis;
    float Angle;
    float Magnitude;

    /**
     * @brief Constructs quaternion polar coordinates and normalizes the axis.
     * @param axis Imaginary-axis direction.
     * @param angle Quaternion polar angle in radians.
     * @param magnitude Quaternion magnitude.
     */
    QPolar(Vector3 axis, float angle, float magnitude = 1) : Axis(axis.Normalized()), Angle(angle), Magnitude(magnitude) {
    }
};

} // namespace E::M
