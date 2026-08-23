#pragma once
#include "Coordinates/HyperSpherical.hpp"

namespace E::M {
struct Quaternion {
    float w, x, y, z;

    Quaternion();
    Quaternion(float all);
    Quaternion(float w, float x, float y, float z);

    void Conjugate() const;
    float MagnitudeSquared() const;
    float Magnitude() const;

    Quaternion operator+(const Quaternion& p) const;
    Quaternion operator*(const Quaternion& p) const;

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

    static const Quaternion Identity;
};
} // namespace E::M
