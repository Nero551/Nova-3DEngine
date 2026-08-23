#pragma once

namespace E::M {
struct HyperSpherical {
    float Elevation;
    float Azimuth;
    float HyperAngle;
    float Magnitude;

    HyperSpherical(float elevation, float azimuth, float hyperAngle, float magnitude = 1) :
        Elevation(elevation), Azimuth(azimuth), HyperAngle(hyperAngle), Magnitude(magnitude) {
    }
};
} // namespace E::M
