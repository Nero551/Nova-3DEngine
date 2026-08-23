#pragma once

struct Spherical {
    float Elevation;
    float Azimuth;
    float Magnitude;

    Spherical(float elevation, float azimuth, float magnitude = 1) :
        Elevation(elevation), Azimuth(azimuth), Magnitude(magnitude) {
    }
};
