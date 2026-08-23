#include "Complex.hpp"

#include "../Common/Comparison.hpp"
#include "../Common/Exponentials.hpp"

namespace E::M {
Complex::Complex() : Real(0), Imaginary(0) {
}
Complex::Complex(float real, float imaginary) : Real(real), Imaginary(imaginary) {
}

Complex Complex::FromPolar(Polar polar) {
    return { polar.Magnitude * std::cos(polar.Angle), polar.Magnitude * std::sin(polar.Angle) };
}

float Complex::MagnitudeSquared() const {
    return Real * Real + Imaginary * Imaginary;
}

float Complex::Magnitude() const {
    return Sqrt(MagnitudeSquared());
}

float Complex::Argument() const {
    return atan2(Imaginary, Real);
}

Complex Complex::Conjugate() const {
    return { Real, -Imaginary };
}

Complex Complex::Inverse() const {
    return Conjugate() / MagnitudeSquared();
}

Complex Complex::Normalized() const {
    return *this / Magnitude();
}

Polar Complex::ToPolar() const {
    return { Argument(), Magnitude() };
}

bool Complex::NearlyEquals(const Complex& b, float epsilon) const {
    return M::NearlyEquals(Real, b.Real, epsilon) && M::NearlyEquals(Imaginary, b.Imaginary, epsilon);
}

bool Complex::operator==(const Complex& b) const {
    return Real == b.Real && Imaginary == b.Imaginary;
}

bool Complex::operator!=(const Complex& b) const {
    return !(*this == b);
}

Complex Complex::operator-() const {
    return -1 * *this;
}

Complex Complex::operator*(const Complex& b) const {
    Complex result;
    result.Real = Real * b.Real - Imaginary * b.Imaginary;
    result.Imaginary = Real * b.Imaginary + Imaginary * b.Real;

    return result;
}

Complex Complex::operator/(const Complex& b) const {
    return *this * b.Inverse();
}

Complex Complex::operator+(const Complex& b) const {
    return { Real + b.Real, Imaginary + b.Imaginary };
}

Complex Complex::operator-(const Complex& b) const {
    return { Real - b.Real, Imaginary - b.Imaginary };
}

Complex& Complex::operator*=(const Complex& b) {
    return *this = *this * b;
}

Complex& Complex::operator+=(const Complex& b) {
    return *this = *this + b;
}

Complex& Complex::operator-=(const Complex& b) {
    return *this = *this - b;
}

Complex& Complex::operator/=(const Complex& b) {
    return *this = *this / b;
}

Complex Complex::operator*(const float scalar) const {
    return { Real * scalar, Imaginary * scalar };
}

Complex Complex::operator/(const float scalar) const {
    return { Real / scalar, Imaginary / scalar };
}

Complex Complex::operator+(float scalar) const {
    return { Real + scalar, Imaginary };
}

Complex Complex::operator-(float scalar) const {
    return { Real - scalar, Imaginary };
}

Complex& Complex::operator*=(float scalar) {
    return *this = *this * scalar;
}

Complex& Complex::operator+=(float scalar) {
    return *this = *this + scalar;
}

Complex& Complex::operator-=(float scalar) {
    return *this = *this - scalar;
}

Complex& Complex::operator/=(float scalar) {
    return *this = *this / scalar;
}

Complex operator*(float scalar, const Complex& a) {
    return a * scalar;
}

Complex operator/(float scalar, const Complex& a) {
    return scalar * a.Inverse();
}

Complex operator+(float scalar, const Complex& a) {
    return a + scalar;
}

Complex operator-(float scalar, const Complex& a) {
    return { scalar - a.Real, -a.Imaginary };
}

std::ostream& operator<<(std::ostream& os, const Complex& complex) {
    os << complex.Real;
    if (complex.Imaginary < 0) {
        os << " - " << -complex.Imaginary << "i";
    }
    else {
        os << " + " << complex.Imaginary << "i";
    }

    return os;
}
} // namespace E::M
