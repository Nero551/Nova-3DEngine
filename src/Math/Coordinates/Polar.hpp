#pragma once

namespace E::M {
/**
 * @brief Represents a two-dimensional point in polar coordinates.
 * A polar coordinate is represented by an angle and a magnitude:
 * @code
 * (r, theta)
 * @endcode
 *
 * where the magnitude represents the distance from the origin and the
 * angle represents the direction measured from the positive x-axis.
 */
struct Polar {
    /** @brief The angle in radians. */
    float Angle;

    /** @brief The distance from the origin. */
    float Magnitude;

    /**
     * @brief Constructs a polar coordinate.
     * @param angle The angle in radians.
     * @param magnitude The distance from the origin.
     */
    Polar(float angle, float magnitude = 1) : Angle(angle), Magnitude(magnitude) {
    }
};
} // namespace E::M
