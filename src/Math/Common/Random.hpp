#pragma once
namespace N::M
{
inline static std::mt19937 rng(std::random_device{}());

constexpr float Random(const float min, const float max)
{
    std::uniform_real_distribution distribution(min, max);
    return distribution(rng);
}

constexpr int Random(const int min, const int max)
{
    std::uniform_int_distribution distribution(min, max);
    return distribution(rng);
}

constexpr float RandomGaussian(const float mean, const float standardDeviation)
{
    std::normal_distribution distribution(mean, standardDeviation);
    return distribution(rng);
}

constexpr unsigned long RandomRaw()
{
    return rng();
}
} // namespace N::M
