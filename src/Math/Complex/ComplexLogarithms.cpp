#include "ComplexLogarithms.hpp"
#include "Math/Common/Logarithms.hpp"

namespace N::M {
Complex CLn(const Complex& z)
{
    Complex result;
    result.Real = Ln(z.Magnitude());
    result.Imaginary = z.Argument();

    return result;
}

Complex CLog(const Complex& base, const Complex& z)
{
    return CLn(z) / CLn(base);
}
} // namespace N::M
