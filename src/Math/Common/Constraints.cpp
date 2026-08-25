#include "Constraints.hpp"

#include "Exponentials.hpp"

namespace N::M {
float Clamp(const float value, const float min, const float max) {
    return std::clamp(value, min, max);
}
} // namespace N::M
