#include "Exponentials.hpp"
#include "Math/Common/Exponentials.hpp"

#include "Math/Common/Logarithms.hpp"
#include "Math/Quaternion/Logarithms.hpp"

namespace N::M {
Quaternion M::Exp(const Quaternion& q) {
    float r = q.Magnitude();
    float theta = q.Angle();
    Vector3 axis = q.Axis();

    float magnitude = Exp(r * std::cos(theta));
    float rsin = r * std::sin(theta);

    Quaternion result;
    result.w = magnitude * std::cos(rsin);
    result.x = magnitude * axis.x * std::sin(rsin);
    result.y = magnitude * axis.y * std::sin(rsin);
    result.z = magnitude * axis.z * std::sin(rsin);

    return result;
}

Quaternion Pow(const float x, const Quaternion& q) {
    return Exp(Ln(x) * q);
}

Quaternion Pow(const Quaternion& q, float power) {
    float magnitude = Pow(q.Magnitude(), power);
    float theta = q.Angle();
    Vector3 axis = q.Axis();
    float sin = std::sin(theta * power);

    Quaternion result;
    result.w = magnitude * std::cos(theta * power);
    result.x = magnitude * axis.x * sin;
    result.y = magnitude * axis.y * sin;
    result.z = magnitude * axis.z * sin;

    return result;
}

Quaternion Pow(const Quaternion& q, const Quaternion& p) {
    return Exp(p * Ln(q));
}

Quaternion Sqrt(const Quaternion& q) {
    return Pow(q, 1.0f / 2.0f);
}
} // namespace N::M
