#pragma once

namespace E::M {
/**
 * @brief Represents a vector using spherical coordinates.
 *
 * Spherical coordinates describe a three-dimensional vector using its
 * magnitude and two angles:
 * - Elevation: angle above the horizontal plane.
 * - Azimuth: angle around the vertical axis.
 * - Magnitude: length of the vector.
 *
 * The angles are expressed in radians.
 */
struct Spherical {
    float Elevation;
    float Azimuth;
    float Magnitude;

    /**
     * @brief Constructs a spherical coordinate.
     * @param elevation Angle above the horizontal plane, in radians.
     * @param azimuth Angle around the vertical axis, in radians.
     * @param magnitude Length of the represented vector.
     */
    Spherical(float elevation, float azimuth, float magnitude = 1) :
        Elevation(elevation), Azimuth(azimuth), Magnitude(magnitude) {
    }
};
} // namespace E::M
