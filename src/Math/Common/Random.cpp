#include "Random.hpp"

namespace N::M {
static std::mt19937 rng(std::random_device{}());

float Random(const float min, const float max) {
    std::uniform_real_distribution distribution(min, max);
    return distribution(rng);
}

int Random(const int min, const int max) {
    std::uniform_int_distribution distribution(min, max);
    return distribution(rng);
}

float RandomGaussian(const float mean, const float standardDeviation) {
    std::normal_distribution distribution(mean, standardDeviation);
    return distribution(rng);
}

unsigned long RandomRaw() {
    return rng();
}
} // namespace N::M
