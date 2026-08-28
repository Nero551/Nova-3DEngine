#pragma once

#include "Math/Vector/Vector3.hpp"
#include "Matrix2.hpp"

namespace N::M {
struct Matrix4;

/**
 * @brief 3x3 floating-point matrix.
 *
 * Matrix convention:
 * - Storage: row-major.
 * - Vectors: column vectors.
 * - Vector multiplication: M * v.
 * - The rightmost transformation is applied first.
 *
 * A Matrix3 can represent:
 * - 3D linear transformations (rotation, scale, etc.).
 * - 2D affine transformations using homogeneous coordinates.
 *
 * 2D transformation layout:
 *
 *     [ x' ]   [ m00 m01 m02 ] [ x ]
 *     [ y' ] = [ m10 m11 m12 ] [ y ]
 *     [ 1  ]   [ m20 m21 m22 ] [ 1 ]
 *
 * Therefore, 2D translation occupies the last column.
 */
struct Matrix3 {
    float m[3][3] = {};

    /**
     * @brief Creates a zero matrix.
     */
    Matrix3() = default;

    /**
     * @brief Creates a matrix with every element set to the same value.
     *
     * @param mAll Value assigned to every element.
     */
    Matrix3(float mAll);

    /**
     * @brief Creates a matrix from its individual elements.
     *
     * Elements are specified in row-major order:
     *
     *     [ m00 m01 m02 ]
     *     [ m10 m11 m12 ]
     *     [ m20 m21 m22 ]
     */
    Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);

    /**
     * @brief Applies a scale transformation.
     *
     * @param scale X, Y, and Z scale factors.
     */
    [[nodiscard]] Matrix3 Scale(const Vector3& scale) const;

    /**
     * @brief Applies a rotation around the X axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix3 RotateX(float radian) const;

    /**
     * @brief Applies a rotation around the Y axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix3 RotateY(float radian) const;

    /**
     * @brief Applies a rotation around the Z axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix3 RotateZ(float radian) const;

    /**
     * @brief Applies an Euler rotation.
     *
     * Rotations are applied in XYZ order:
     * X, then Y, then Z.
     *
     * @param eulerRotation Rotation angles around X, Y, and Z in radians.
     */
    [[nodiscard]] Matrix3 Rotate(const Vector3& eulerRotation) const;

    /**
     * @brief Applies a rotation around an arbitrary axis.
     *
     * @param axis Axis of rotation.
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix3 RotateAroundAxis(const Vector3& axis, float radian) const;

    /**
     * @brief Applies a 2D translation using homogeneous coordinates.
     *
     * @param trans X and Y translation.
     */
    [[nodiscard]] Matrix3 Translate(const Vector2& trans) const;

    /**
     * @brief Calculates the determinant of this matrix.
     */
    [[nodiscard]] float Determinant() const;

    /**
     * @brief Returns the transpose of this matrix.
     */
    [[nodiscard]] Matrix3 Transpose() const;

    /**
     * @brief Returns the inverse of this matrix.
     *
     * If the matrix is not invertible, a warning is sent
     * and Identity is returned.
     */
    [[nodiscard]] Matrix3 Inverse() const;

    /**
     * @brief Returns the minor matrix produced by removing a row and column.
     *
     * @param row Row to remove.
     * @param col Column to remove.
     */
    [[nodiscard]] Matrix2 Minor(int row, int col) const;

    /**
     * @brief Compares two matrices using an absolute error tolerance.
     *
     * @param mat3 Matrix to compare against.
     * @param epsilon Maximum allowed difference between corresponding elements.
     */
    [[nodiscard]] bool NearlyEquals(const Matrix3& mat3, float epsilon = EPSILON) const;

    /** @brief returns a matrix 4 version of this matrix 3. with all the extra values = 0 */
    [[nodiscard]] Matrix4 ToMatrix4() const;

    Matrix3 operator+(const Matrix3& mat3) const;
    Matrix3 operator-(const Matrix3& mat3) const;
    Matrix3 operator*(const Matrix3& mat3) const;

    Matrix3& operator+=(const Matrix3& mat3);
    Matrix3& operator-=(const Matrix3& mat3);
    Matrix3& operator*=(const Matrix3& mat3);

    /**
     * @brief Multiplies this matrix by a column vector.
     */
    Vector3 operator*(const Vector3& vec3) const;

    Matrix3 operator*(float scalar) const;
    Matrix3 operator/(float scalar) const;

    Matrix3& operator*=(float scalar);
    Matrix3& operator/=(float scalar);

    /**
     * @brief Returns the additive inverse of this matrix.
     */
    Matrix3 operator-() const;

    bool operator==(const Matrix3& mat3) const;
    bool operator!=(const Matrix3& mat3) const;

    /**
     * @brief Matrix containing only zeros.
     */
    static const Matrix3 Zero;

    /**
     * @brief 3x3 identity matrix.
     */
    static const Matrix3 Identity;

    friend Matrix3 operator*(float scalar, const Matrix3& mat3);
    friend std::ostream& operator<<(std::ostream& os, const Matrix3& mat3);
};
} // namespace N::M
