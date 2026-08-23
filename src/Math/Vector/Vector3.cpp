#include "Vector3.hpp"

#include <ostream>

#include "Math/Common/Comparison.hpp"
#include "Math/Common/Interpolation.hpp"

namespace E::M {
Vector3 Vector3::FromSpherical(Spherical spherical) {
    Vector3 result;
    result.x = spherical.Magnitude * std::cos(spherical.Elevation) * std::cos(spherical.Azimuth);
    result.z = spherical.Magnitude * std::cos(spherical.Elevation) * std::sin(spherical.Azimuth);
    result.y = spherical.Magnitude * std::sin(spherical.Elevation);

    return result;
}

Vector3::Vector3() : x(0), y(0), z(0) {
}

Vector3::Vector3(const float all) : x(all), y(all), z(all) {
}

Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {
}

//?Operators

//?Methods
float Vector3::LengthSquared() const {
    return x * x + y * y + z * z;
}

float Vector3::Length() const {
    return std::sqrt(LengthSquared());
}

Vector3 Vector3::Normalized() const {
    float length = Length();
    if (length == 0) {
        return Zero;
    }
    return { x / length, y / length, z / length };
}

float Vector3::Dot(const Vector3& vec3) const {
    return x * vec3.x + y * vec3.y + z * vec3.z;
}

Vector3 Vector3::Cross(const Vector3& vec3) const {
    return { y * vec3.z - z * vec3.y, z * vec3.x - x * vec3.z, x * vec3.y - y * vec3.x };
}

float Vector3::Distance(const Vector3& vec3) const {
    return (*this - vec3).Length();
}

float Vector3::Elevation() const {
    return std::asin(Normalized().y);
}

float Vector3::Azimuth() const {
    return std::atan2(z, x);
}

Spherical Vector3::ToSpherical() const {
    return { Elevation(), Azimuth(), Length() };
}

bool Vector3::NearlyEquals(const Vector3& vec3, const float epsilon) const {
    return M::NearlyEquals(x, vec3.x, epsilon) && M::NearlyEquals(y, vec3.y, epsilon) && M::NearlyEquals(z, vec3.z, epsilon);
}

bool Vector3::IsParallelTo(const Vector3& vec3) const {
    if (*this == Zero || vec3 == Zero) {
        return false;
    }

    return Cross(vec3).LengthSquared() < EPSILON * EPSILON;
}

bool Vector3::IsPerpendicularTo(const Vector3& vec3) const {
    if (*this == Zero || vec3 == Zero) {
        return false;
    }

    return std::abs(Dot(vec3)) < EPSILON;
}

Vector3 Vector3::Lerp(const Vector3& vec3, const float t) const {
    return { M::Lerp(x, vec3.x, t), M::Lerp(y, vec3.y, t), M::Lerp(z, vec3.z, t) };
}

//*Vectors
Vector3 Vector3::operator+(const Vector3& vec3) const {
    return { x + vec3.x, y + vec3.y, z + vec3.z };
}

Vector3 Vector3::operator-(const Vector3& vec3) const {
    return { x - vec3.x, y - vec3.y, z - vec3.z };
}

Vector3 Vector3::operator*(const Vector3& vec3) const {
    return { x * vec3.x, y * vec3.y, z * vec3.z };
}

Vector3 Vector3::operator/(const Vector3& vec3) const {
    return { x / vec3.x, y / vec3.y, z / vec3.z };
}

Vector3& Vector3::operator+=(const Vector3& vec3) {
    return *this = *this + vec3;
}

Vector3& Vector3::operator-=(const Vector3& vec3) {
    return *this = *this - vec3;
}

Vector3& Vector3::operator*=(const Vector3& vec3) {
    return *this = *this * vec3;
}

Vector3& Vector3::operator/=(const Vector3& vec3) {
    return *this = *this / vec3;
}

//*Scalars
Vector3 Vector3::operator+(const float scalar) const {
    return { x + scalar, y + scalar, z + scalar };
}

Vector3 Vector3::operator-(const float scalar) const {
    return { x - scalar, y - scalar, z - scalar };
}

Vector3 Vector3::operator*(const float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

Vector3 Vector3::operator/(const float scalar) const {
    return { x / scalar, y / scalar, z / scalar };
}

Vector3& Vector3::operator+=(const float scalar) {
    return *this = *this + scalar;
}

Vector3& Vector3::operator-=(const float scalar) {
    return *this = *this - scalar;
}

Vector3& Vector3::operator*=(const float scalar) {
    return *this = *this * scalar;
}

Vector3& Vector3::operator/=(const float scalar) {
    return *this = *this / scalar;
}

Vector3 Vector3::operator-() const {
    return -1 * *this;
}

//*Equality
bool Vector3::operator==(const Vector3& vec3) const {
    return x == vec3.x && y == vec3.y && z == vec3.z;
}

bool Vector3::operator!=(const Vector3& vec3) const {
    return !(*this == vec3);
}

Vector3 operator+(const float scalar, const Vector3& vec3) {
    return vec3 + scalar;
}

Vector3 operator-(const float scalar, const Vector3& vec3) {
    return { scalar - vec3.x, scalar - vec3.y, scalar - vec3.z };
}

Vector3 operator*(const float scalar, const Vector3& vec3) {
    return vec3 * scalar;
}

Vector3 operator/(const float scalar, const Vector3& vec3) {
    return { scalar / vec3.x, scalar / vec3.y, scalar / vec3.z };
}

//* Others
std::ostream& operator<<(std::ostream& os, const Vector3& vec3) {
    os << "(" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")";
    return os;
}

//?Statics
Vector3 const Vector3::Zero = Vector3(0, 0, 0);
Vector3 const Vector3::One = Vector3(1, 1, 1);
Vector3 const Vector3::Up = Vector3(0, 1, 0);
Vector3 const Vector3::Right = Vector3(1, 0, 0);
Vector3 const Vector3::Forward = Vector3(0, 0, 1);
} // namespace E::M
