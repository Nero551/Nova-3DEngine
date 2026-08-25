#include "Vector2.hpp"

#include <ostream>

#include "Math/Common/Comparison.hpp"
#include "Math/Common/Interpolation.hpp"

namespace N::M {
Vector2 Vector2::FromPolar(Polar polar) {
    return { polar.Magnitude * std::cos(polar.Angle), polar.Magnitude * std::sin(polar.Angle) };
}

Vector2::Vector2() : x(0), y(0) {
}

Vector2::Vector2(const float all) : x(all), y(all) {
}

Vector2::Vector2(const float x, const float y) : x(x), y(y) {
}

//?Operators

//?Methods
float Vector2::LengthSquared() const {
    return x * x + y * y;
}

float Vector2::Length() const {
    return std::sqrt(LengthSquared());
}

float Vector2::Angle() const {
    return std::atan2(y, x);
}

Vector2 Vector2::Normalized() const {
    float length = Length();
    if (length == 0) {
        return Zero;
    }
    return { x / length, y / length };
}

float Vector2::Dot(const Vector2& vec2) const {
    return x * vec2.x + y * vec2.y;
}

Vector2 Vector2::Lerp(const Vector2& vec2, const float t) const {
    return { M::Lerp(x, vec2.x, t), M::Lerp(y, vec2.y, t) };
}

float Vector2::Distance(const Vector2& vec2) const {
    return (*this - vec2).Length();
}

float Vector2::StereoProject() const {
    float r = Length();
    return r * x / (r - y);
}

Polar Vector2::ToPolar() const {
    return { Angle(), Length() };
}

bool Vector2::NearlyEquals(const Vector2& vec2, const float epsilon) const {
    return M::NearlyEquals(x, vec2.x, epsilon) && M::NearlyEquals(y, vec2.y, epsilon);
}

//* Vectors
Vector2 Vector2::operator+(const Vector2& vec2) const {
    return { x + vec2.x, y + vec2.y };
}

Vector2 Vector2::operator-(const Vector2& vec2) const {
    return { x - vec2.x, y - vec2.y };
}

Vector2 Vector2::operator*(const Vector2& vec2) const {
    return { x * vec2.x, y * vec2.y };
}

Vector2& Vector2::operator+=(const Vector2& vec2) {
    return *this = *this + vec2;
}

Vector2& Vector2::operator-=(const Vector2& vec2) {
    return *this = *this - vec2;
}

Vector2& Vector2::operator*=(const Vector2& vec2) {
    return *this = *this * vec2;
}

//* Scalars
Vector2 Vector2::operator+(const float scalar) const {
    return { x + scalar, y + scalar };
}

Vector2 Vector2::operator-(const float scalar) const {
    return { x - scalar, y - scalar };
}

Vector2 Vector2::operator*(const float scalar) const {
    return { x * scalar, y * scalar };
}

Vector2 Vector2::operator/(const float scalar) const {
    return { x / scalar, y / scalar };
}

Vector2& Vector2::operator+=(const float scalar) {
    return *this = *this + scalar;
}

Vector2& Vector2::operator-=(const float scalar) {
    return *this = *this - scalar;
}

Vector2& Vector2::operator*=(const float scalar) {
    return *this = *this * scalar;
}

Vector2& Vector2::operator/=(const float scalar) {
    return *this = *this / scalar;
}

Vector2 Vector2::operator-() const {
    return -1 * *this;
}

//* Equality
bool Vector2::operator==(const Vector2& vec2) const {
    return x == vec2.x && y == vec2.y;
}

bool Vector2::operator!=(const Vector2& vec2) const {
    return !(*this == vec2);
}

Vector2 operator+(const float scalar, const Vector2& vec2) {
    return vec2 + scalar;
}

Vector2 operator-(const float scalar, const Vector2& vec2) {
    return { scalar - vec2.x, scalar - vec2.y };
}

Vector2 operator*(const float scalar, const Vector2& vec2) {
    return vec2 * scalar;
}

Vector2 operator/(const float scalar, const Vector2& vec2) {
    return { scalar / vec2.x, scalar / vec2.y };
}

//* Others
std::ostream& operator<<(std::ostream& os, const Vector2& vec2) {
    os << "(" << vec2.x << ", " << vec2.y << ")";
    return os;
}

//?Statics
Vector2 const Vector2::Zero = Vector2(0, 0);
} // namespace N::M
