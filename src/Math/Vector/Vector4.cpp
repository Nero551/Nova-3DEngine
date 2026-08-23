#include "Vector4.hpp"

#include <ostream>

#include "Math/Common/Comparison.hpp"
#include "Math/Common/Interpolation.hpp"

namespace E::M {
Vector4 Vector4::FromHyperSpherical(HyperSpherical hyperSpherical) {
    const float m = hyperSpherical.Magnitude;
    Vector4 result;
    result.x = m * std::cos(hyperSpherical.Elevation) * std::cos(hyperSpherical.Azimuth) * std::cos(hyperSpherical.HyperAngle);
    result.y = m * std::sin(hyperSpherical.Elevation) * std::cos(hyperSpherical.HyperAngle);
    result.z = m * std::cos(hyperSpherical.Elevation) * std::sin(hyperSpherical.Azimuth) * std::cos(hyperSpherical.HyperAngle);
    result.w = m * std::sin(hyperSpherical.HyperAngle);

    return result;
}

Vector4::Vector4() : x(0), y(0), z(0), w(0) {
}

Vector4::Vector4(const float all) : x(all), y(all), z(all), w(all) {
}

Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {
}

//?Operators

//?Methods
float Vector4::LengthSquared() const {
    return x * x + y * y + z * z + w * w;
}

float Vector4::Length() const {
    return std::sqrt(LengthSquared());
}

Vector4 Vector4::Normalized() const {
    const float length = Length();
    if (length == 0) {
        return Zero;
    }
    return { x / length, y / length, z / length, w / length };
}

float Vector4::Dot(const Vector4& vec4) const {
    return x * vec4.x + y * vec4.y + z * vec4.z + w * vec4.w;
}

Vector4 Vector4::Lerp(const Vector4& vec4, const float t) const {
    return { M::Lerp(x, vec4.x, t), M::Lerp(y, vec4.y, t), M::Lerp(z, vec4.z, t), M::Lerp(w, vec4.w, t) };
}

float Vector4::Distance(const Vector4& vec4) const {
    return (*this - vec4).Length();
}

float Vector4::Elevation() const {
    return std::asin(Normalized().y / std::cos(HyperAngle()));
}

float Vector4::Azimuth() const {
    return std::atan2(z, x);
}

float Vector4::HyperAngle() const {
    return std::asin(Normalized().w);
}

HyperSpherical Vector4::ToHyperSpherical() const {
    return { Elevation(), Azimuth(), HyperAngle(), Length() };
}

bool Vector4::NearlyEquals(const Vector4& vec4, const float epsilon) const {
    return M::NearlyEquals(x, vec4.x, epsilon) && M::NearlyEquals(y, vec4.y, epsilon) && M::NearlyEquals(z, vec4.z, epsilon) &&
        M::NearlyEquals(w, vec4.w, epsilon);
}

//*Vectors
Vector4 Vector4::operator+(const Vector4& vec4) const {
    return { x + vec4.x, y + vec4.y, z + vec4.z, w + vec4.w };
}

Vector4 Vector4::operator-(const Vector4& vec4) const {
    return { x - vec4.x, y - vec4.y, z - vec4.z, w - vec4.w };
}

Vector4 Vector4::operator*(const Vector4& vec4) const {
    return { x * vec4.x, y * vec4.y, z * vec4.z, w * vec4.w };
}

Vector4& Vector4::operator+=(const Vector4& vec4) {
    return *this = *this + vec4;
}

Vector4& Vector4::operator-=(const Vector4& vec4) {
    return *this = *this - vec4;
}

Vector4& Vector4::operator*=(const Vector4& vec4) {
    return *this = *this * vec4;
}

//*Scalars
Vector4 Vector4::operator+(const float scalar) const {
    return { x + scalar, y + scalar, z + scalar, w + scalar };
}

Vector4 Vector4::operator-(const float scalar) const {
    return { x - scalar, y - scalar, z - scalar, w - scalar };
}

Vector4 Vector4::operator*(const float scalar) const {
    return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Vector4 Vector4::operator/(const float scalar) const {
    return { x / scalar, y / scalar, z / scalar, w / scalar };
}

Vector4& Vector4::operator+=(const float scalar) {
    return *this = *this + scalar;
}

Vector4& Vector4::operator-=(const float scalar) {
    return *this = *this - scalar;
}

Vector4& Vector4::operator*=(const float scalar) {
    return *this = *this * scalar;
}

Vector4& Vector4::operator/=(const float scalar) {
    return *this = *this / scalar;
}

Vector4 Vector4::operator-() const {
    return -1 * *this;
}

//*Equality
bool Vector4::operator==(const Vector4& vec4) const {
    return x == vec4.x && y == vec4.y && z == vec4.z && w == vec4.w;
}

bool Vector4::operator!=(const Vector4& vec4) const {
    return !(*this == vec4);
}

Vector4 operator+(const float scalar, const Vector4& vec4) {
    return vec4 + scalar;
}

Vector4 operator-(const float scalar, const Vector4& vec4) {
    return { scalar - vec4.x, scalar - vec4.y, scalar - vec4.z, scalar - vec4.w };
}

Vector4 operator*(const float scalar, const Vector4& vec4) {
    return vec4 * scalar;
}

Vector4 operator/(const float scalar, const Vector4& vec4) {
    return { scalar / vec4.x, scalar / vec4.y, scalar / vec4.z, scalar / vec4.w };
}

//* Others
std::ostream& operator<<(std::ostream& os, const Vector4& vec4) {
    os << "(" << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w << ")";
    return os;
}

//?Statics
Vector4 const Vector4::Zero = Vector4(0, 0, 0, 0);
} // namespace E::M
