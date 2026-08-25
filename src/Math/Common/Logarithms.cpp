#include "Logarithms.hpp"

#include <complex>

namespace N::M {
float Ln(const float x) {
    return std::log(x);
}

float Log(const float base, const float x) {
    return Ln(x) / Ln(base);
}
} // namespace N::M
