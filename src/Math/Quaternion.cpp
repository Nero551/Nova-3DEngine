#include "Quaternion.hpp"

#include "Common/Comparison.hpp"
#include "Common/Exponentials.hpp"


namespace E::M {
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

bool Quaternion::operator==(const Quaternion& p) const {
    return w == p.w && x == p.x && y == p.y && z == p.z;
}

bool Quaternion::operator!=(const Quaternion& p) const {
    return !(*this == p);
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
