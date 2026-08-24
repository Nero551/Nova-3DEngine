#pragma once
#include "Quaternion.hpp"

namespace E::M {
/** @brief Computes the exponential of a quaternion. */
Quaternion Exp(const Quaternion& q);

/** @brief Raises a real number to a quaternion power. */
Quaternion Pow(float x, const Quaternion& q);

/** @brief Raises a quaternion to a real power. */
Quaternion Pow(const Quaternion& q, float power);

/** @brief Raises a quaternion to a quaternion power using p * Ln(q). */
Quaternion Pow(const Quaternion& q, const Quaternion& p);

/** @brief Computes the square root of a quaternion. */
Quaternion Sqrt(const Quaternion& q);
} // namespace E::M
