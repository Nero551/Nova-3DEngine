#include "ComplexExponentials.hpp"
#include "ComplexLogarithms.hpp"
#include "Math/Common/Exponentials.hpp"
#include "Math/Common/Logarithms.hpp"

namespace N::M
{
Complex CPow(float x, const Complex& z)
{
    return CExp(Ln(x) * z);
}

Complex CPow(const Complex& z, float power)
{
    Complex result;

    float magnitude = Pow(z.Magnitude(), power);
    float theta = z.Argument() * power;

    result.Real = magnitude * std::cos(theta);
    result.Imaginary = magnitude * std::sin(theta);
    return result;
}

Complex CPow(const Complex& z, const Complex& w)
{
    return CExp(w * CLn(z));
}

Complex CSqrt(const Complex& z)
{
    return CPow(z, 1.0f / 2.0f);
}

Complex CExp(const Complex& z)
{
    float m = Exp(z.Real);
    return {m * std::cos(z.Imaginary), m * std::sin(z.Imaginary)};
}
} // namespace N::M
