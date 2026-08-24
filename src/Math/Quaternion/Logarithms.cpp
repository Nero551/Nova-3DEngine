#include "Logarithms.hpp"
#include "Math/Common/Logarithms.hpp"

#include <complex>

#include "Math/Common/Exponentials.hpp"

namespace E::M {
Quaternion Ln(const Quaternion& q) {
    Quaternion result;
    Vector3 axis = q.Axis();

    // mathematically, this = m * | sin(rsin(x)) |
    float imaginaryMagnitude = Sqrt(q.x * q.x + q.y * q.y + q.z * q.z);

    // and q.w is m * cos(rsin(x))
    // their arctan returns rsin. which is the original magnitude * sin(original angle)
    float rsin = std::atan2(imaginaryMagnitude, q.w);

    // axis is unchanged by exponentiation so "u" remains the same
    // so this is u * original magnitude * sin(original angle), which
    result.w = Ln(q.Magnitude());
    result.x = axis.x * rsin;
    result.y = axis.y * rsin;
    result.z = axis.z * rsin;

    return result;
}
} // namespace E::M
