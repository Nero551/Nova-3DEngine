#pragma once
#include "Quaternion.hpp"

namespace E::M {
Quaternion Exp(const Quaternion& q);
Quaternion Pow(float x, const Quaternion& q);
Quaternion Pow(const Quaternion& q, float power);
} // namespace E::M
