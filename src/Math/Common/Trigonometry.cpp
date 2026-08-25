#include "Trigonometry.hpp"

#include "Comparison.hpp"
#include "Constants.hpp"
#include "Exponentials.hpp"

namespace N::M {
float Rad(const float deg) {
    return deg * PI / 180;
}

float Deg(const float rad) {
    return rad * 180 / PI;
}

float DSin(const float deg) {
    return std::sin(Rad(deg));
}

float DCos(const float deg) {
    return std::cos(Rad(deg));
}

float DTan(const float deg) {
    return std::tan(Rad(deg));
}
} // namespace N::M
