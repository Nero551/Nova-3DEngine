#include "Quaternion.hpp"

#include "../Common/Comparison.hpp"
#include "../Common/Exponentials.hpp"


namespace E::M {
Quaternion Quaternion::FromQPolar(const QPolar& qPolar) {
    Quaternion result;
    float m = qPolar.Magnitude;
    float sine = std::sin(qPolar.Angle);
    result.w = m * std::cos(qPolar.Angle);
    result.x = m * (qPolar.Axis.x * sine);
    result.y = m * (qPolar.Axis.y * sine);
    result.z = m * (qPolar.Axis.z * sine);

    return result;
}

Quaternion Quaternion::FromMatrix3(const Matrix3& mat3) {
    float trace = mat3.m[0][0] + mat3.m[1][1] + mat3.m[2][2];

    Quaternion rotation;

    if (trace > 0) {
        float s = Sqrt(trace + 1.0f) * 2.0f;

        rotation.w = 0.25f * s;
        rotation.x = (mat3.m[2][1] - mat3.m[1][2]) / s;
        rotation.y = (mat3.m[0][2] - mat3.m[2][0]) / s;
        rotation.z = (mat3.m[1][0] - mat3.m[0][1]) / s;
    }
    else if (mat3.m[0][0] > mat3.m[1][1] && mat3.m[0][0] > mat3.m[2][2]) {
        float s = Sqrt(1.0f + mat3.m[0][0] - mat3.m[1][1] - mat3.m[2][2]) * 2.0f;

        rotation.w = (mat3.m[2][1] - mat3.m[1][2]) / s;
        rotation.x = 0.25f * s;
        rotation.y = (mat3.m[0][1] + mat3.m[1][0]) / s;
        rotation.z = (mat3.m[0][2] + mat3.m[2][0]) / s;
    }
    else if (mat3.m[1][1] > mat3.m[2][2]) {
        float s = Sqrt(1.0f + mat3.m[1][1] - mat3.m[0][0] - mat3.m[2][2]) * 2.0f;

        rotation.w = (mat3.m[0][2] - mat3.m[2][0]) / s;
        rotation.x = (mat3.m[0][1] + mat3.m[1][0]) / s;
        rotation.y = 0.25f * s;
        rotation.z = (mat3.m[1][2] + mat3.m[2][1]) / s;
    }
    else {
        float s = Sqrt(1.0f + mat3.m[2][2] - mat3.m[0][0] - mat3.m[1][1]) * 2.0f;

        rotation.w = (mat3.m[1][0] - mat3.m[0][1]) / s;
        rotation.x = (mat3.m[0][2] + mat3.m[2][0]) / s;
        rotation.y = (mat3.m[1][2] + mat3.m[2][1]) / s;
        rotation.z = 0.25f * s;
    }

    // Convert the rotation quaternion from half-angle form
    // to the full-angle quaternion representation.
    return rotation * rotation;
}

Quaternion Quaternion::FromEulerXYZ(const Vector3& euler) {
    Matrix3 rotation = Matrix3::Identity;
    rotation = rotation.Rotate(euler);
    return FromMatrix3(rotation);
}

Quaternion::Quaternion() : w(0), x(0), y(0), z(0) {
}

Quaternion::Quaternion(const float all) : w(all), x(all), y(all), z(all) {
}

Quaternion::Quaternion(const float w, const float x, const float y, const float z) : w(w), x(x), y(y), z(z) {
}

Quaternion Quaternion::Conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

float Quaternion::MagnitudeSquared() const {
    return w * w + x * x + y * y + z * z;
}

float Quaternion::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

Quaternion Quaternion::Inverse() const {
    return Conjugate() / MagnitudeSquared();
}

Quaternion Quaternion::Normalized() const {
    return *this / Magnitude();
}

Vector3 Quaternion::Transform(const Vector3& vec3) const {
    Quaternion p = { 0, vec3.x, vec3.y, vec3.z };
    Quaternion q = FromQPolar({ Axis(), Angle() / 2, Magnitude() });
    Quaternion result = q * p * q.Inverse();

    return { result.x, result.y, result.z };
}

float Quaternion::Angle() const {
    Quaternion q = Normalized();

    float imaginaryMagnitude = Sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    return std::atan2(imaginaryMagnitude, q.w);
}

Vector3 Quaternion::Axis() const {
    Quaternion q = Normalized();
    float sine = std::sin(Angle());
    Vector3 axis;
    axis.x = q.x / sine;
    axis.y = q.y / sine;
    axis.z = q.z / sine;
    return axis;
}

QPolar Quaternion::ToQPolar() const {
    return { Axis(), Angle(), Magnitude() };
}

Matrix4 Quaternion::ToMatrix4() const {
    Matrix4 result = Matrix4::Identity;

    if (iszero(Angle())) {
        return Matrix4::Identity;
    }

    return result.RotateAroundAxis(Axis(), Angle());
}

Vector3 Quaternion::ToEulerXYZ() const {
    const Matrix4 matrix = ToMatrix4();
    Vector3 result;
    result.x = std::atan2(matrix.m[2][1], matrix.m[2][2]);
    result.y = std::asin(-matrix.m[2][0]);
    result.z = std::atan2(matrix.m[1][0], matrix.m[0][0]);
    return result;
}

bool Quaternion::NearlyEquals(const Quaternion& p, const float epsilon) const {
    return M::NearlyEquals(w, p.w, epsilon) && M::NearlyEquals(x, p.x, epsilon) && M::NearlyEquals(y, p.y, epsilon) &&
        M::NearlyEquals(z, p.z, epsilon);
}

Quaternion Quaternion::operator-() const {
    return { -w, -x, -y, -z };
}

Quaternion Quaternion::operator+(const Quaternion& p) const {
    Quaternion result;
    result.w = w + p.w;
    result.x = x + p.x;
    result.y = y + p.y;
    result.z = z + p.z;
    return result;
}

Quaternion Quaternion::operator-(const Quaternion& p) const {
    return *this + (-p);
}

Quaternion& Quaternion::operator*=(const Quaternion& p) {
    return *this = *this * p;
}

Quaternion& Quaternion::operator/=(const Quaternion& p) {
    return *this = *this / p;
}

Quaternion& Quaternion::operator+=(const Quaternion& p) {
    return *this = *this + p;
}

Quaternion& Quaternion::operator-=(const Quaternion& p) {
    return *this = *this - p;
}

Quaternion Quaternion::operator*(const Quaternion& p) const {
    Quaternion result;
    result.w = (w * p.w) - (x * p.x) - (y * p.y) - (z * p.z);
    result.x = (w * p.x) + (x * p.w) + (y * p.z) - (z * p.y);
    result.y = (w * p.y) - (x * p.z) + (y * p.w) + (z * p.x);
    result.z = (w * p.z) + (x * p.y) - (y * p.x) + (z * p.w);

    return result;
}

Quaternion Quaternion::operator/(const Quaternion& p) const {
    return *this * p.Inverse();
}

Quaternion Quaternion::operator*(float scalar) const {
    return { w * scalar, x * scalar, y * scalar, z * scalar };
}

Quaternion Quaternion::operator/(float scalar) const {
    return { w / scalar, x / scalar, y / scalar, z / scalar };
}

Quaternion Quaternion::operator+(float scalar) const {
    return { w + scalar, x, y, z };
}

Quaternion Quaternion::operator-(float scalar) const {
    return { w - scalar, x, y, z };
}

Quaternion& Quaternion::operator*=(float scalar) {
    return *this = *this * scalar;
}

Quaternion& Quaternion::operator/=(float scalar) {
    return *this = *this / scalar;
}

Quaternion& Quaternion::operator+=(float scalar) {
    return *this = *this + scalar;
}

Quaternion& Quaternion::operator-=(float scalar) {
    return *this = *this - scalar;
}

bool Quaternion::operator==(const Quaternion& p) const {
    return w == p.w && x == p.x && y == p.y && z == p.z;
}

bool Quaternion::operator!=(const Quaternion& p) const {
    return !(*this == p);
}

Quaternion operator*(float scalar, const Quaternion& q) {
    return q * scalar;
}

Quaternion operator/(float scalar, const Quaternion& q) {
    return scalar * q.Inverse();
}

Quaternion operator+(float scalar, const Quaternion& q) {
    return q + scalar;
}

Quaternion operator-(float scalar, const Quaternion& q) {
    return { scalar - q.w, -q.x, -q.y, -q.z };
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
    os << q.w;

    if (q.x < 0) {
        os << " - " << -q.x << "i";
    }
    else {
        os << " + " << q.x << "i";
    }

    if (q.y < 0) {
        os << " - " << -q.y << "j";
    }
    else {
        os << " + " << q.y << "j";
    }

    if (q.z < 0) {
        os << " - " << -q.z << "k";
    }
    else {
        os << " + " << q.z << "k";
    }

    return os;
}

Quaternion const Quaternion::Identity = Quaternion(1, 0, 0, 0);
} // namespace E::M
