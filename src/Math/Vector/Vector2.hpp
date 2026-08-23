#pragma once

#include "Math/Common/Constants.hpp"
#include "Math/Coordinates/Polar.hpp"

// TODO- add FromPolar() and Angle(), as a path to polar cords from cartesian
// TODO- add ToPolar()
//  for vector3 make Elevation() and Azimuth() methods and FromSpherical()
//  for vector4 make Elevation(), Azimuth() , HyperAngle() , also do FromHyperSpherical.
namespace E::M {
/**
 * @brief 2D floating-point vector.
 *
 * Components:
 * - x: horizontal component.
 * - y: vertical component.
 *
 * Vectors are treated as column vectors when used with matrices.
 */
struct Vector2 {
    float x;
    float y;

    /**
     * @brief Constructs a vector from polar coordinates (r,theta)
     * @param polar Polar coords to use.
     */
    [[nodiscard]] static Vector2 FromPolar(Polar polar);

    /**
     * @brief Creates a zero vector.
     */
    Vector2();

    /**
     * @brief Creates a vector with both components set to the same value.
     *
     * @param all Value assigned to x and y.
     */
    Vector2(float all);

    /**
     * @brief Creates a vector from its individual components.
     */
    Vector2(float x, float y);

    /**
     * @brief Returns the squared length of the vector.
     *
     * This avoids the square root performed by Length().
     * Prefer this when only comparing vector lengths.
     */
    [[nodiscard]] float LengthSquared() const;

    /**
     * @brief Returns the length (magnitude) of the vector.
     */
    [[nodiscard]] float Length() const;

    /**
     * @brief Returns the angle of the vector measured from the positive x-axis.
     * @return The angle in radians, in the range [-PI, PI].
     */
    [[nodiscard]] float Angle() const;

    /**
     * @brief Returns a normalized copy of the vector.
     *
     * The resulting vector has a length of 1.
     */
    [[nodiscard]] Vector2 Normalized() const;

    /**
     * @brief Calculates the dot product with another vector.
     */
    [[nodiscard]] float Dot(const Vector2& vec2) const;

    /**
     * @brief Linearly interpolates between this vector and another vector.
     *
     * A t value of 0 returns this vector.
     * A t value of 1 returns vec3.
     *
     * Values outside [0, 1] extrapolate beyond the endpoints.
     *
     * @param vec2 Target vector.
     * @param t Interpolation amount.
     */
    [[nodiscard]] Vector2 Lerp(const Vector2& vec2, float t) const;

    /**
     * @brief Calculates the distance to another vector.
     */
    [[nodiscard]] float Distance(const Vector2& vec2) const;

    /**
     * @brief returns vector in polar coords
     */
    [[nodiscard]] Polar ToPolar() const;

    /**
     * @brief Compares two vectors using an absolute error tolerance.
     *
     * @param vec2 Vector to compare against.
     * @param epsilon Maximum allowed difference between corresponding components.
     */
    [[nodiscard]] bool NearlyEquals(const Vector2& vec2, float epsilon = EPSILON) const;

    Vector2 operator+(const Vector2& vec2) const;
    Vector2 operator-(const Vector2& vec2) const;

    /**
     * @brief Performs component-wise multiplication.
     *
     * This is not a dot or cross product.
     *
     * Example:
     *     (2, 3) * (4, 5) = (8, 15)
     */
    Vector2 operator*(const Vector2& vec2) const;

    Vector2& operator+=(const Vector2& vec2);
    Vector2& operator-=(const Vector2& vec2);
    Vector2& operator*=(const Vector2& vec2);

    /**
     * @brief Adds a scalar to every component.
     */
    Vector2 operator+(float scalar) const;

    /**
     * @brief Subtracts a scalar from every component.
     */
    Vector2 operator-(float scalar) const;

    /**
     * @brief Multiplies every component by a scalar.
     */
    Vector2 operator*(float scalar) const;

    /**
     * @brief Divides every component by a scalar.
     */
    Vector2 operator/(float scalar) const;

    Vector2& operator+=(float scalar);
    Vector2& operator-=(float scalar);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    /**
     * @brief Returns the negated vector.
     */
    Vector2 operator-() const;

    bool operator==(const Vector2& vec2) const;
    bool operator!=(const Vector2& vec2) const;

    friend Vector2 operator+(float scalar, const Vector2& vec2);
    friend Vector2 operator-(float scalar, const Vector2& vec2);
    friend Vector2 operator*(float scalar, const Vector2& vec2);
    friend Vector2 operator/(float scalar, const Vector2& vec2);

    friend std::ostream& operator<<(std::ostream& os, const Vector2& vec2);

    /**
     * @brief Zero vector (0, 0).
     */
    static const Vector2 Zero;
};
} // namespace E::M
