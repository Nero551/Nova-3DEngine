#include "Exponentials.hpp"
#include "Logarithms.hpp"
#include "Math/Common/Exponentials.hpp"
#include "Math/Common/Logarithms.hpp"

namespace N::M {
Complex Pow(float x, const Complex& z) {
    return Exp(Ln(x) * z);
}

Complex Pow(const Complex& z, float power) {
    Complex result;

    float magnitude = Pow(z.Magnitude(), power);
    float theta = z.Argument() * power;

    result.Real = magnitude * std::cos(theta);
    result.Imaginary = magnitude * std::sin(theta);
    return result;
}

Complex Pow(const Complex& z, const Complex& w) {
    return Exp(w * Ln(z));
}

Complex Sqrt(const Complex& z) {
    return Pow(z, 1.0f / 2.0f);
}

Complex Exp(const Complex& z) {
    float m = Exp(z.Real);
    return { m * std::cos(z.Imaginary), m * std::sin(z.Imaginary) };
}
} // namespace N::M
