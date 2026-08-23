#pragma once

namespace E::M {
/**
 * @brief Represents a vector using hyperspherical coordinates.
 *
 * Hyperspherical coordinates describe a four-dimensional vector using
 * its magnitude and three angles:
 * - Elevation: angle above the horizontal plane.
 * - Azimuth: angle around the vertical axis.
 * - HyperAngle: angle determining the vector's component along the fourth
 *   dimension.
 * - Magnitude: length of the vector.
 *
 * The angles are expressed in radians.
 */
struct HyperSpherical {
    float Elevation;
    float Azimuth;
    float HyperAngle;
    float Magnitude;

    /**
     * @brief Constructs a hyperspherical coordinate.
     *
     * @param elevation Angle above the horizontal plane, in radians.
     * @param azimuth Angle around the vertical axis, in radians.
     * @param hyperAngle Angle determining the fourth-dimensional component, in radians.
     * @param magnitude Length of the represented vector.
     */
    HyperSpherical(float elevation, float azimuth, float hyperAngle, float magnitude = 1) :
        Elevation(elevation), Azimuth(azimuth), HyperAngle(hyperAngle), Magnitude(magnitude) {
    }
};
} // namespace E::M
