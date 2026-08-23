#pragma once
#include "Common/Constants.hpp"
#include "Coordinates/AxisAngle.hpp"
#include "Coordinates/HyperSpherical.hpp"
#include "Vector/Vector3.hpp"

namespace E::M {
struct Quaternion {
    float w, x, y, z;

    static Quaternion FromAxisAngle(AxisAngle axisAngle);
    Quaternion();
    Quaternion(float all);
    Quaternion(float w, float x, float y, float z);

    Quaternion Conjugate() const;
    float MagnitudeSquared() const;
    float Magnitude() const;
    Quaternion Inverse() const;
    Quaternion Normalized() const;
    Vector3 Transform(const Vector3 vec3);
    float Angle() const;
    Vector3 Axis() const;
    AxisAngle ToAxisAngle() const;

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
