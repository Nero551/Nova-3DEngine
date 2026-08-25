#pragma once
#include "../Common/Constants.hpp"
#include "../Coordinates/QPolar.hpp"
#include "../Matrix/Matrix4.hpp"
#include "../Vector/Vector3.hpp"

namespace N::M {
/**
 * @brief Represents a quaternion number.
 *
 * A quaternion is a four-dimensional number of the form:
 * `q = w + xi + yj + zk`, where `i² = j² = k² = ijk = -1`.
 *
 * Quaternions are treated primarily as a number system and algebraic
 * structure. Their ability to represent and manipulate 3D rotations
 * follows naturally from quaternion multiplication and conjugation.
 *
 * A quaternion can also be represented in polar form:
 * `q = m(cos(θ) + u sin(θ))`, where `m` is the magnitude, `θ` is the
 * quaternion's polar angle, and `u` is a unit vector in the imaginary part.
 *
 * @note This class uses the full quaternion polar angle rather than the
 * conventional half-angle used by many rotation-only quaternion APIs.
 * Conversion to rotation matrices and vector transformations accounts
 * for the relationship between quaternion and spatial rotation angles.
 */
struct Quaternion {
    // TODO: Investigate the principal branch of Quaternion Ln/Exp.
    // Ln(Exp(q)) == q only when the imaginary-vector magnitude is within
    // the principal range (< PI). Outside it, the logarithm wraps by 2*PI.
    float w, x, y, z;

    /**
     * @brief Constructs a quaternion from quaternion polar coordinates.
     *
     * Creates `q = m(cos(θ) + u sin(θ))`, where `m` is the magnitude,
     * `θ` is the angle, and `u` is the imaginary-axis direction.
     *
     * @param qPolar Quaternion polar representation.
     * @return The corresponding quaternion.
     */
    static Quaternion FromQPolar(const QPolar& qPolar);

    /**
     * @brief Constructs a quaternion from a 3x3 rotation matrix.
     *
     * The matrix is interpreted as a spatial rotation. The intermediate
     * quaternion uses the conventional half-angle representation before
     * being converted into this class's full-angle representation.
     *
     * @param mat3 Rotation matrix.
     * @return Quaternion representing the same rotation.
     */
    static Quaternion FromMatrix3(const Matrix3& mat3);

    /**
     * @brief Constructs a quaternion from XYZ Euler angles.
     *
     * Rotations are composed as `q = qz * qy * qx`, corresponding to
     * the X, Y, then Z Euler components.
     *
     * @param euler Euler angles `(x, y, z)`.
     * @return Quaternion representing the composed rotation.
     */
    static Quaternion FromEulerXYZ(const Vector3& euler);

    /** @brief Constructs the zero quaternion. */
    Quaternion();

    /** @brief Constructs a quaternion with all components equal to `all`. */
    Quaternion(float all);

    /**
     * @brief Constructs a quaternion from its four components.
     * @param w Real component.
     * @param x Coefficient of the `i` imaginary unit.
     * @param y Coefficient of the `j` imaginary unit.
     * @param z Coefficient of the `k` imaginary unit.
     */
    Quaternion(float w, float x, float y, float z);

    /**
     * @brief Returns the quaternion conjugate.
     * For `q = w + xi + yj + zk`, the conjugate is `q* = w - xi - yj - zk`.
     */
    Quaternion Conjugate() const;

    /** @brief Returns the squared magnitude: `|q|² = w² + x² + y² + z²`. */
    float MagnitudeSquared() const;

    /** @brief Returns the magnitude: `|q| = sqrt(w² + x² + y² + z²)`. */
    float Magnitude() const;

    /** @brief Returns the multiplicative inverse: `q⁻¹ = q* / |q|²`. */
    Quaternion Inverse() const;

    /** @brief Returns a normalized copy of the quaternion with magnitude one. */
    Quaternion Normalized() const;

    /** @brief Dot product of 2 quaternions */
    float Dot(const Quaternion& p) const;

    /**
     * @brief Transforms a vector using the quaternion as a rotation.
     *
     * The full-angle representation is converted to the corresponding
     * half-angle rotation quaternion before applying `v' = qvq⁻¹`.
     *
     * @param vec3 Vector to transform.
     * @return Transformed vector.
     */
    Vector3 Transform(const Vector3& vec3) const;

    /** @brief Returns the quaternion polar angle `θ` from `q = cos(θ) + u sin(θ)`. */
    float Angle() const;

    /** @brief Returns the normalized imaginary-axis direction `u` from `q = m(cos(θ) + u sin(θ))`. */
    Vector3 Axis() const;

    /** @brief Converts the quaternion to polar representation containing its axis, angle, and magnitude. */
    QPolar ToQPolar() const;

    /**
     * @brief Converts the quaternion to a 4x4 rotation matrix.
     *
     * The quaternion's full-angle representation is converted into
     * the corresponding spatial rotation.
     *
     * @return 4x4 matrix representing the quaternion's rotation.
     */
    Matrix4 ToMatrix4() const;

    /**
     * @brief Converts the quaternion to XYZ Euler angles.
     *
     * Uses the same XYZ convention as FromEulerXYZ(), with
     * `q = qz * qy * qx`.
     *
     * @return Euler angles `(x, y, z)` in radians.
     */
    Vector3 ToEulerXYZ() const;

    /**
     * @brief Compares two quaternions using an epsilon tolerance.
     *
     * @param p Quaternion to compare against.
     * @param epsilon Maximum allowed component-wise difference.
     * @return True if the quaternions are approximately equal.
     */
    bool NearlyEquals(const Quaternion& p, float epsilon = EPSILON) const;

    /** @brief Tests exact component-wise equality. */
    bool operator==(const Quaternion& p) const;

    /** @brief Tests exact component-wise inequality. */
    bool operator!=(const Quaternion& p) const;

    /** @brief Returns the additive inverse: `-q = -w - xi - yj - zk`. */
    Quaternion operator-() const;

    /**
     * @brief Multiplies two quaternions.
     * Quaternion multiplication is non-commutative; in general, `pq != qp`.
     */
    Quaternion operator*(const Quaternion& p) const;

    /** @brief Divides this quaternion by another: `q / p = q * p⁻¹`. */
    Quaternion operator/(const Quaternion& p) const;

    /** @brief Adds two quaternions component-wise. */
    Quaternion operator+(const Quaternion& p) const;

    /** @brief Subtracts two quaternions component-wise. */
    Quaternion operator-(const Quaternion& p) const;

    /** @brief Multiplies this quaternion by another quaternion in-place. */
    Quaternion& operator*=(const Quaternion& p);

    /** @brief Divides this quaternion by another quaternion in-place. */
    Quaternion& operator/=(const Quaternion& p);

    /** @brief Adds another quaternion to this quaternion in-place. */
    Quaternion& operator+=(const Quaternion& p);

    /** @brief Subtracts another quaternion from this quaternion in-place. */
    Quaternion& operator-=(const Quaternion& p);

    /**
     * @brief Multiplies every component by a scalar.
     * @param scalar Scalar multiplier.
     * @return Scaled quaternion.
     */
    Quaternion operator*(float scalar) const;

    /**
     * @brief Divides every component by a scalar.
     * @param scalar Scalar divisor.
     * @return Scaled quaternion.
     */
    Quaternion operator/(float scalar) const;

    /** @brief Adds a scalar to the real component: `(w + xi + yj + zk) + s = (w + s) + xi + yj + zk`. */
    Quaternion operator+(float scalar) const;

    /** @brief Subtracts a scalar from the real component. */
    Quaternion operator-(float scalar) const;

    /** @brief Multiplies this quaternion by a scalar in-place. */
    Quaternion& operator*=(float scalar);

    /** @brief Divides this quaternion by a scalar in-place. */
    Quaternion& operator/=(float scalar);

    /** @brief Adds a scalar to the real component in-place. */
    Quaternion& operator+=(float scalar);

    /** @brief Subtracts a scalar from the real component in-place. */
    Quaternion& operator-=(float scalar);

    /** @brief Multiplies a quaternion by a scalar. */
    friend Quaternion operator*(float scalar, const Quaternion& q);

    /** @brief Divides a scalar by a quaternion. */
    friend Quaternion operator/(float scalar, const Quaternion& q);

    /** @brief Adds a scalar to a quaternion's real component. */
    friend Quaternion operator+(float scalar, const Quaternion& q);

    /** @brief Subtracts a quaternion from a scalar. */
    friend Quaternion operator-(float scalar, const Quaternion& q);

    /**
     * @brief Writes a quaternion in algebraic form to a stream.
     *
     * For example: `1 + 2i - 3j + 4k`.
     */
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

    /** @brief Multiplicative identity quaternion: `1 + 0i + 0j + 0k`. */
    static const Quaternion Identity;
};
} // namespace N::M
