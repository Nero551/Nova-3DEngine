#pragma once
#include "Common/Constants.hpp"
#include "Coordinates/HyperSpherical.hpp"

namespace E::M {
struct Quaternion {
    float w, x, y, z;

    Quaternion();
    Quaternion(float all);
    Quaternion(float w, float x, float y, float z);

    Quaternion Conjugate() const;
    float MagnitudeSquared() const;
    float Magnitude() const;
    Quaternion Inverse() const;
    Quaternion Normalized() const;
    bool NearlyEquals(const Quaternion& p, float epsilon = EPSILON) const;

    bool operator==(const Quaternion& p) const;
    bool operator!=(const Quaternion& p) const;

    Quaternion operator-() const;
    Quaternion operator*(const Quaternion& p) const;
    Quaternion operator/(const Quaternion& p) const;
    Quaternion operator+(const Quaternion& p) const;
    Quaternion operator-(const Quaternion& p) const;

    Quaternion& operator*=(const Quaternion& p);
    Quaternion& operator/=(const Quaternion& p);
    Quaternion& operator+=(const Quaternion& p);
    Quaternion& operator-=(const Quaternion& p);

    Quaternion operator*(float scalar) const;
    Quaternion operator/(float scalar) const;
    Quaternion operator+(float scalar) const;
    Quaternion operator-(float scalar) const;

    Quaternion& operator*=(float scalar);
    Quaternion& operator/=(float scalar);
    Quaternion& operator+=(float scalar);
    Quaternion& operator-=(float scalar);

    friend Quaternion operator*(float scalar, const Quaternion& q);
    friend Quaternion operator/(float scalar, const Quaternion& q);
    friend Quaternion operator+(float scalar, const Quaternion& q);
    friend Quaternion operator-(float scalar, const Quaternion& q);

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

    static const Quaternion Identity;
};
} // namespace E::M
