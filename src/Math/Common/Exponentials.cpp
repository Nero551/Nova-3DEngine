#include "Exponentials.hpp"

#include "Logarithms.hpp"
#include "Utilities/Logger.hpp"

namespace N::M {
float Pow(const float x, const float power) {
    return std::pow(x, power);
}

float Sqrt(const float x) {
    return std::sqrt(x);
}


float Exp(const float x) {
    return std::exp(x);
}

float Factorial(int x) {
    if (x < 0)
        U::Logger::Error("Factorial requires a non-negative integer");

    float result = 1.0f;

    for (int i = x; i > 0; i--)
        result *= static_cast<float>(i);

    return result;
}
} // namespace N::M
