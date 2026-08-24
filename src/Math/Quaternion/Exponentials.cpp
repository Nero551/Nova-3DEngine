#include "Exponentials.hpp"
#include "Math/Common/Exponentials.hpp"

#include "Math/Common/Logarithms.hpp"

namespace E::M {
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
    float rc = Pow(q.Magnitude(), power);
    float theta = q.Angle();
    float rsin = rc * std::sin(theta * power);

    Quaternion result;
    result.w = rc * std::cos(theta * power);
    result.x = q.x * rsin;
    result.y = q.y * rsin;
    result.z = q.z * rsin;

    return result;
}
} // namespace E::M
