#pragma once
#include "Complex.hpp"

namespace E::M {
/** @brief Raises a real number to a complex power. */
Complex Pow(float x, const Complex& z);

/** @brief Raises a complex number to a real power. */
Complex Pow(const Complex& z, float power);

/** @brief Raises a complex number to a complex power. */
Complex Pow(const Complex& z, const Complex& w);

/** @brief Computes the square root of a complex number. */
Complex Sqrt(const Complex& z);

/** @brief Computes the exponential of a complex number. */
Complex Exp(const Complex& z);
} // namespace E::M
