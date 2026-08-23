#pragma once

#include "../Common/Constants.hpp"
#include "../Coordinates/Polar.hpp"

namespace E::M {
/**
 * @brief Represents a complex number in Cartesian form.
 *
 * A complex number is represented as:
 * @code
 * z = Real + Imaginary * i
 * @endcode
 *
 * where @c i is the imaginary unit satisfying:
 * @code
 * i^2 = -1
 * @endcode
 */
struct Complex {
    float Real;
    float Imaginary;

    /** @brief Constructs a complex number with both components set to zero. */
    Complex();

    /**
     * @brief Constructs a complex number from its real and imaginary components.
     * @param real The real component.
     * @param imaginary The imaginary component.
     */
    Complex(float real, float imaginary);

    /**
     * @brief Constructs a complex number from polar coordinates.
     * @param polar The magnitude and angle of the complex number.
     * @return The equivalent complex number in Cartesian form.
     */
    static Complex FromPolar(Polar polar);

    /**
     * @brief Returns the squared magnitude of the complex number.
     * @return The squared magnitude.
     */
    float MagnitudeSquared() const;

    /**
     * @brief Returns the magnitude (modulus) of the complex number.
     * @return The magnitude.
     */
    float Magnitude() const;

    /**
     * @brief Returns the argument of the complex number.
     *
     * The argument is the angle between the positive real axis and
     * the vector represented by the complex number.
     *
     * The returned angle is in radians and is in the range
     * [-PI, PI].
     *
     * @return The principal argument in radians.
     */
    float Argument() const;

    /**
     * @brief Returns the complex conjugate.
     * @return The complex conjugate.
     */
    Complex Conjugate() const;

    /**
     * @brief Returns the multiplicative inverse.
     * @return The multiplicative inverse.
     */
    Complex Inverse() const;

    /**
     * @brief Returns a normalized complex number.
     *
     * The returned complex number has magnitude 1 while preserving
     * the original argument.
     *
     * @return The normalized complex number.
     */
    Complex Normalized() const;

    /**
     * @brief Converts the complex number to polar coordinates.
     *
     * The returned polar coordinates contain the magnitude and
     * principal argument of the complex number.
     *
     * @return The equivalent polar representation.
     */
    Polar ToPolar() const;

    /**
     * @brief Tests whether two complex numbers are approximately equal.
     *
     * @param b The complex number to compare against.
     * @param epsilon The maximum allowed difference between components.
     * @return @c true if both components are approximately equal.
     */
    bool NearlyEquals(const Complex& b, float epsilon = EPSILON) const;

    bool operator==(const Complex& b) const;
    bool operator!=(const Complex& b) const;

    Complex operator-() const;
    Complex operator*(const Complex& b) const;
    Complex operator/(const Complex& b) const;
    Complex operator+(const Complex& b) const;
    Complex operator-(const Complex& b) const;

    Complex& operator*=(const Complex& b);
    Complex& operator/=(const Complex& b);
    Complex& operator+=(const Complex& b);
    Complex& operator-=(const Complex& b);

    Complex operator*(float scalar) const;
    Complex operator/(float scalar) const;
    Complex operator+(float scalar) const;
    Complex operator-(float scalar) const;

    Complex& operator*=(float scalar);
    Complex& operator/=(float scalar);
    Complex& operator+=(float scalar);
    Complex& operator-=(float scalar);


    friend Complex operator*(float scalar, const Complex& a);
    friend Complex operator/(float scalar, const Complex& a);
    friend Complex operator+(float scalar, const Complex& a);
    friend Complex operator-(float scalar, const Complex& a);

    friend std::ostream& operator<<(std::ostream& os, const Complex& complex);
};
} // namespace E::M
