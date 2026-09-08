#pragma once
#include "Complex.hpp"

namespace N::M
{
/** @brief Raises a real number to a complex power. */
Complex CPow(float x, const Complex &z);

/** @brief Raises a complex number to a real power. */
Complex CPow(const Complex &z, float power);

/** @brief Raises a complex number to a complex power. */
Complex CPow(const Complex &z, const Complex &w);

/** @brief Computes the square root of a complex number. */
Complex CSqrt(const Complex &z);

/** @brief Computes the exponential of a complex number. */
Complex CExp(const Complex &z);
} // namespace N::M
