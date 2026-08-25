#pragma once
namespace N::M {
float Lerp(float a, float b, float t);
float EaseInQuad(float t);
float EaseOutQuad(float t);
float EaseInOutQuad(float t);

float EaseInCubic(float t);
float EaseOutCubic(float t);
float EaseInOutCubic(float t);
} // namespace N::M
