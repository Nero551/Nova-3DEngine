#pragma once

#include "Math/Vector/Vector4.hpp"
#include "Matrix3.hpp"

namespace N::M {
/**
 * @brief 4x4 floating-point matrix.
 *
 * Matrix convention:
 * - Storage: row-major.
 * - Vectors: column vectors.
 * - Vector multiplication: M * v.
 * - The rightmost transformation is applied first.
 *
 * Matrix4 uses homogeneous coordinates and can represent:
 * - 3D translation.
 * - 3D scale.
 * - 3D rotation.
 * - Combined transformations.
 * - Orthographic and perspective projections.
 * - Camera/view transformations.
 *
 * Vector multiplication:
 *
 *     [ x' ]   [ m00 m01 m02 m03 ] [ x ]
 *     [ y' ] = [ m10 m11 m12 m13 ] [ y ]
 *     [ z' ]   [ m20 m21 m22 m23 ] [ z ]
 *     [ w' ]   [ m30 m31 m32 m33 ] [ w ]
 *
 * Translation occupies the last column.
 */
struct Matrix4 {
    float m[4][4] = {};

    /**
     * @brief Creates a zero matrix.
     */
    Matrix4() = default;

    /**
     * @brief Creates a matrix with every element set to the same value.
     *
     * @param mAll Value assigned to every element.
     */
    Matrix4(float mAll);

    /**
     * @brief Creates a matrix from its individual elements.
     *
     * Elements are specified in row-major order:
     *
     *     [ m00 m01 m02 m03 ]
     *     [ m10 m11 m12 m13 ]
     *     [ m20 m21 m22 m23 ]
     *     [ m30 m31 m32 m33 ]
     */
    Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21,
        float m22, float m23, float m30, float m31, float m32, float m33);

    /**
     * @brief Applies a 3D translation.
     *
     * @param translation Translation along the X, Y, and Z axes.
     */
    [[nodiscard]] Matrix4 Translate(const Vector3& translation) const;

    /**
     * @brief Applies a 3D scale.
     *
     * @param scale Scale factors along the X, Y, and Z axes.
     */
    [[nodiscard]] Matrix4 Scale(const Vector3& scale) const;

    /**
     * @brief Applies a rotation around the X axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix4 RotateX(float radian) const;

    /**
     * @brief Applies a rotation around the Y axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix4 RotateY(float radian) const;

    /**
     * @brief Applies a rotation around the Z axis.
     *
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix4 RotateZ(float radian) const;

    /**
     * @brief Applies an Euler rotation.
     *
     * Rotations are applied in XYZ order:
     * X, then Y, then Z.
     *
     * @param eulerRotation Rotation angles around X, Y, and Z in radians.
     */
    [[nodiscard]] Matrix4 Rotate(const Vector3& eulerRotation) const;

    /**
     * @brief Applies a rotation around an arbitrary axis.
     *
     * @param axis Axis of rotation.
     * @param radian Rotation angle in radians.
     */
    [[nodiscard]] Matrix4 RotateAroundAxis(const Vector3& axis, float radian) const;

    /**
     * @brief Extracts the upper-left 3x3 portion of the matrix.
     *
     * This removes the translation and homogeneous-coordinate components.
     */
    [[nodiscard]] Matrix3 ToMatrix3() const;

    /**
     * @brief Calculates the determinant of this matrix.
     */
    [[nodiscard]] float Determinant() const;

    /**
     * @brief Returns the transpose of this matrix.
     */
    [[nodiscard]] Matrix4 Transpose() const;

    /**
     * @brief Returns the inverse of this matrix.
     *
     * If the matrix is not invertible, an warning is sent
     * and Identity is returned.
     */
    [[nodiscard]] Matrix4 Inverse() const;

    /**
     * @brief Returns the minor matrix produced by removing a row and column.
     *
     * @param row Row to remove.
     * @param col Column to remove.
     */
    [[nodiscard]] Matrix3 Minor(int row, int col) const;

    /**
     * @brief Compares two matrices using an absolute error tolerance.
     *
     * @param mat4 Matrix to compare against.
     * @param epsilon Maximum allowed difference between corresponding elements.
     */
    [[nodiscard]] bool NearlyEquals(const Matrix4& mat4, float epsilon = EPSILON) const;

    Matrix4 operator+(const Matrix4& mat4) const;
    Matrix4 operator-(const Matrix4& mat4) const;
    Matrix4 operator*(const Matrix4& mat4) const;

    Matrix4& operator+=(const Matrix4& mat4);
    Matrix4& operator-=(const Matrix4& mat4);
    Matrix4& operator*=(const Matrix4& mat4);

    /**
     * @brief Multiplies this matrix by a column vector.
     */
    Vector4 operator*(const Vector4& vec4) const;

    Matrix4 operator*(float scalar) const;
    Matrix4 operator/(float scalar) const;

    Matrix4& operator*=(float scalar);
    Matrix4& operator/=(float scalar);

    /**
     * @brief Returns the additive inverse of this matrix.
     */
    Matrix4 operator-() const;

    bool operator==(const Matrix4& mat4) const;
    bool operator!=(const Matrix4& mat4) const;

    /**
     * @brief Creates an orthographic projection matrix.
     *
     * @param left Left clipping plane.
     * @param right Right clipping plane.
     * @param bottom Bottom clipping plane.
     * @param top Top clipping plane.
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     */
    static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

    /**
     * @brief Creates a perspective projection matrix.
     *
     * @param fovRad field of view in radians.
     * @param aspectRatio Viewport width divided by height.
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     */
    static Matrix4 Perspective(float fovRad, float aspectRatio, float near, float far);

    /**
     * @brief Creates a view matrix looking from one position toward another.
     *
     * @param pos Camera position.
     * @param target Point the camera is looking toward.
     * @param up Approximate world-up direction.
     */
    static Matrix4 LookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

    /**
     * @brief Matrix containing only zeros.
     */
    static const Matrix4 Zero;

    /**
     * @brief 4x4 identity matrix.
     */
    static const Matrix4 Identity;
};

Matrix4 operator*(float scalar, const Matrix4& mat4);

std::ostream& operator<<(std::ostream& os, const Matrix4& mat4);
} // namespace N::M
