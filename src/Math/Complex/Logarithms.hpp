#pragma once
#include "Math/Complex/Complex.hpp"

namespace N::M {
/** @brief Computes the natural logarithm of a complex number. */
Complex Ln(const Complex& z);

/** @brief Computes the logarithm of a complex number with a complex base. */
Complex Log(const Complex& base, const Complex& z);
} // namespace N::M
