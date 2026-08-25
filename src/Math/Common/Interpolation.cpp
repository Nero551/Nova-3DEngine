#include "Interpolation.hpp"

#include "Exponentials.hpp"

namespace N::M {
float Lerp(const float a, const float b, const float t) {
    return a + (b - a) * t;
}

float EaseInQuad(const float t) {
    return t * t;
}

float EaseOutQuad(const float t) {
    return 1 - Pow(1 - t, 2);
}

float EaseInOutQuad(const float t) {
    if (t < 0.5f)
        return 0.5f * EaseInQuad(t * 2.0f);

    return 0.5f + 0.5f * EaseOutQuad((t - 0.5f) * 2.0f);
}

float EaseInCubic(const float t) {
    return t * t * t;
}

float EaseOutCubic(const float t) {
    return 1 - Pow(1 - t, 3);
}

float EaseInOutCubic(const float t) {
    if (t < 0.5f)
        return 4 * EaseInCubic(t);

    return 0.5f + 0.5f * EaseOutCubic((t - 0.5f) * 2.0f);
}
} // namespace N::M
