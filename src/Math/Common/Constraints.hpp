#pragma once

namespace N::M {
float Clamp(float value, float min, float max);

/** @brief Smooth maximum */
float SMax(float a, float b, float bias);
} // namespace N::M
