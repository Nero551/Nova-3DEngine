#pragma once
namespace N::M {
float Random(float min = 0.0f, float max = 1.0f);
int Random(int min, int max);
float RandomGaussian(float mean, float standardDeviation);
unsigned long RandomRaw();
} // namespace N::M
