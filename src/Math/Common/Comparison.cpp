#include "Comparison.hpp"

namespace N::M {
bool NearlyEquals(const float a, const float b, const float epsilon) {
    return std::abs(a - b) <= epsilon;
}
} // namespace N::M
