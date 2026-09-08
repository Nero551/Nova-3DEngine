#pragma once
#include "Quaternion.hpp"

namespace N::M
{
/** @brief Computes the exponential of a quaternion. */
Quaternion QExp(const Quaternion &q);

/** @brief Raises a real number to a quaternion power. */
Quaternion QPow(float x, const Quaternion &q);

/** @brief Raises a quaternion to a real power. */
Quaternion QPow(const Quaternion &q, float power);

/** @brief Raises a quaternion to a quaternion power using p * Ln(q). */
Quaternion QPow(const Quaternion &q, const Quaternion &p);

/** @brief Computes the square root of a quaternion. */
Quaternion QSqrt(const Quaternion &q);
} // namespace N::M
