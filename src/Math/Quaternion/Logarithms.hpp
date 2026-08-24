#pragma once
#include "Quaternion.hpp"

namespace E::M {
Quaternion Ln(const Quaternion& q);
Quaternion Log(const Quaternion& base, const Quaternion& q);
} // namespace E::M
