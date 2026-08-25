#pragma once
#include "Math/Common/Constants.hpp"
#include "Math/Coordinates/HyperSpherical.hpp"
#include "Vector3.hpp"

namespace N::M {
/**
 * @brief 4D floating-point vector.
 *
 * Stores four floating-point components: x, y, z, and w.
 *
 * Vector operations are component-wise unless otherwise specified.
 *
 * Vector4 can be used as a general 4D mathematical vector or as a
 * homogeneous coordinate when used with Matrix4 transformations.
 */
struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    static Vector4 FromHyperSpherical(HyperSpherical hyperSpherical);

    Vector4();

    Vector4(float all);

    Vector4(float x, float y, float z, float w);

    /**
     * @brief Returns the squared magnitude of the vector.
     *
     * Equivalent to Dot(*this), but avoids the square root performed
     * by Length().
     *
     * Useful when comparing vector magnitudes without needing the
     * actual length.
     */
    [[nodiscard]] float LengthSquared() const;

    /**
     * @brief Returns the magnitude (length) of the vector.
     */
    [[nodiscard]] float Length() const;

    /**
     * @brief Returns a normalized copy of the vector.
     *
     * The returned vector has a length of 1 while preserving the
     * direction of the original vector.
     */
    [[nodiscard]] Vector4 Normalized() const;

    /**
     * @brief Returns the dot product between two vectors.
     */
    [[nodiscard]] float Dot(const Vector4& vec4) const;

    /**
     * @brief Linearly interpolates between this vector and another vector.
     *
     * A t value of 0 returns this vector.
     * A t value of 1 returns vec4.
     *
     * Values outside [0, 1] extrapolate beyond the endpoints.
     *
     * @param vec4 Target vector.
     * @param t Interpolation amount.
     */
    [[nodiscard]] Vector4 Lerp(const Vector4& vec4, float t) const;

    /**
     * @brief Returns the Euclidean distance between two vectors.
     */
    [[nodiscard]] float Distance(const Vector4& vec4) const;

    [[nodiscard]] Vector3 StereoProject() const;

    [[nodiscard]] float Elevation() const;
    [[nodiscard]] float Azimuth() const;
    [[nodiscard]] float HyperAngle() const;
    [[nodiscard]] HyperSpherical ToHyperSpherical() const;

    /**
     * @brief Compares two vectors using an epsilon tolerance.
     *
     * Unlike operator==, this comparison is suitable for floating-point
     * calculations where exact equality is unreliable.
     *
     * @param vec4 Vector to compare against.
     * @param epsilon Maximum allowed component difference.
     */
    [[nodiscard]] bool NearlyEquals(const Vector4& vec4, float epsilon = EPSILON) const;

    Vector4 operator+(const Vector4& vec4) const;

    Vector4 operator-(const Vector4& vec4) const;

    Vector4 operator*(const Vector4& vec4) const;

    Vector4& operator+=(const Vector4& vec4);

    Vector4& operator-=(const Vector4& vec4);

    Vector4& operator*=(const Vector4& vec4);

    Vector4 operator+(float scalar) const;

    Vector4 operator-(float scalar) const;

    Vector4 operator*(float scalar) const;

    Vector4 operator/(float scalar) const;

    Vector4& operator+=(float scalar);

    Vector4& operator-=(float scalar);

    Vector4& operator*=(float scalar);

    Vector4& operator/=(float scalar);

    Vector4 operator-() const;

    bool operator==(const Vector4& vec4) const;

    bool operator!=(const Vector4& vec4) const;

    friend Vector4 operator+(float scalar, const Vector4& vec4);

    friend Vector4 operator-(float scalar, const Vector4& vec4);

    friend Vector4 operator*(float scalar, const Vector4& vec4);

    friend Vector4 operator/(float scalar, const Vector4& vec4);

    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec4);

    static const Vector4 Zero;
};
} // namespace N::M
