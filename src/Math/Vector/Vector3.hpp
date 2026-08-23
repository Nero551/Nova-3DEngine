#pragma once

#include "Math/Common/Constants.hpp"
#include "Math/Coordinates/Spherical.hpp"

namespace E::M {
/**
 * @brief 3D floating-point vector.
 *
 * Coordinate convention:
 * - X: right
 * - Y: up
 * - Z: forward
 *
 * The coordinate system follows the right-hand rule.
 *
 * Vectors are treated as column vectors when used with matrices.
 */
struct Vector3 {
    float x;
    float y;
    float z;

    /** @brief constructs a vector3 from spherical coords (elevation , azimuth) */
    [[nodiscard]] static Vector3 FromSpherical(Spherical spherical);

    /**
     * @brief Creates a zero vector.
     */
    Vector3();

    /**
     * @brief Creates a vector with all components set to the same value.
     *
     * @param all Value assigned to x, y, and z.
     */
    Vector3(float all);

    /**
     * @brief Creates a vector from its individual components.
     */
    Vector3(float x, float y, float z);

    /**
     * @brief Returns the squared length of the vector.
     *
     * Avoids the square root performed by Length().
     * Prefer this when only comparing vector lengths.
     */
    [[nodiscard]]
    float LengthSquared() const;

    /**
     * @brief Returns the length (magnitude) of the vector.
     */
    [[nodiscard]] float Length() const;

    /**
     * @brief Returns a normalized copy of the vector.
     *
     * The resulting vector has a length of 1.
     */
    [[nodiscard]] Vector3 Normalized() const;

    /**
     * @brief Calculates the dot product with another vector.
     */
    [[nodiscard]] float Dot(const Vector3& vec3) const;

    /**
     * @brief Calculates the cross product with another vector.
     *
     * The resulting vector is perpendicular to both input vectors
     * and follows the right-hand rule.
     */
    [[nodiscard]] Vector3 Cross(const Vector3& vec3) const;

    /**
     * @brief Calculates the distance to another vector.
     */
    [[nodiscard]] float Distance(const Vector3& vec3) const;

    /**
     * @brief Returns the angle of the vector measured from the YX or YZ planes.
     * @return The angle in radians, in the range [-PI, PI].
     */
    [[nodiscard]] float Elevation() const;


    /**
     * @brief Returns the angle of the vector measured from the XZ plane.
     * @return The angle in radians, in the range [-PI, PI].
     */
    [[nodiscard]] float Azimuth() const;

    [[nodiscard]] Spherical ToSpherical() const;

    /**
     * @brief Compares two vectors using an absolute error tolerance.
     *
     * @param vec3 Vector to compare against.
     * @param epsilon Maximum allowed difference between components.
     */
    [[nodiscard]] bool NearlyEquals(const Vector3& vec3, float epsilon = EPSILON) const;

    /**
     * @brief Determines whether two non-zero vectors are parallel.
     *
     * Parallel vectors may point in the same or opposite direction.
     *
     * Zero vectors are never considered parallel.
     */
    [[nodiscard]] bool IsParallelTo(const Vector3& vec3) const;

    /**
     * @brief Determines whether two non-zero vectors are perpendicular.
     *
     * Zero vectors are never considered perpendicular.
     */
    [[nodiscard]] bool IsPerpendicularTo(const Vector3& vec3) const;

    /**
     * @brief Linearly interpolates between this vector and another vector.
     *
     * A t value of 0 returns this vector.
     * A t value of 1 returns vec3.
     *
     * Values outside [0, 1] extrapolate beyond the endpoints.
     *
     * @param vec3 Target vector.
     * @param t Interpolation amount.
     */
    [[nodiscard]] Vector3 Lerp(const Vector3& vec3, float t) const;

    Vector3 operator+(const Vector3& vec3) const;
    Vector3 operator-(const Vector3& vec3) const;

    /**
     * @brief Performs component-wise multiplication.
     */
    Vector3 operator*(const Vector3& vec3) const;

    /**
     * @brief Performs component-wise division.
     */
    Vector3 operator/(const Vector3& vec3) const;

    Vector3& operator+=(const Vector3& vec3);
    Vector3& operator-=(const Vector3& vec3);
    Vector3& operator*=(const Vector3& vec3);
    Vector3& operator/=(const Vector3& vec3);

    /**
     * @brief Adds a scalar to every component.
     */
    Vector3 operator+(float scalar) const;

    /**
     * @brief Subtracts a scalar from every component.
     */
    Vector3 operator-(float scalar) const;

    /**
     * @brief Multiplies every component by a scalar.
     */
    Vector3 operator*(float scalar) const;

    /**
     * @brief Divides every component by a scalar.
     */
    Vector3 operator/(float scalar) const;

    Vector3& operator+=(float scalar);
    Vector3& operator-=(float scalar);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    /**
     * @brief Returns the negated vector.
     */
    Vector3 operator-() const;

    bool operator==(const Vector3& vec3) const;
    bool operator!=(const Vector3& vec3) const;

    friend Vector3 operator+(float scalar, const Vector3& vec3);
    friend Vector3 operator-(float scalar, const Vector3& vec3);
    friend Vector3 operator*(float scalar, const Vector3& vec3);
    friend Vector3 operator/(float scalar, const Vector3& vec3);

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec3);

    /**
     * @brief Zero vector (0, 0, 0).
     */
    static const Vector3 Zero;

    /**
     * @brief Unit vector with all components set to 1.
     */
    static const Vector3 One;

    /**
     * @brief Unit vector pointing upward along the Y axis.
     */
    static const Vector3 Up;

    /**
     * @brief Unit vector pointing right along the X axis.
     */
    static const Vector3 Right;

    /**
     * @brief Unit vector pointing forward along the Z axis.
     */
    static const Vector3 Forward;
};
} // namespace E::M
