#pragma once
#include <iosfwd>

#include "Math/Vector/Vector2.hpp"

namespace N::M {
/**
 * @brief 2x2 floating-point matrix.
 *
 * Matrix convention:
 * - Storage: row-major.
 * - Vectors: column vectors.
 * - Vector multiplication: M * v.
 * - Transformations are composed through matrix multiplication.
 *
 * For column vectors, the rightmost transformation is applied first.
 *
 * Example:
 *     M * v
 *
 *     [ m00 m01 ] [ x ]   [ m00*x + m01*y ]
 *     [ m10 m11 ] [ y ] = [ m10*x + m11*y ]
 */
struct Matrix2 {
    float m[2][2] = {};

    /**
     * @brief Creates a zero matrix.
     */
    Matrix2() = default;

    /**
     * @brief Creates a matrix with every element set to the same value.
     *
     * @param mAll Value assigned to every element.
     */
    Matrix2(float mAll);

    /**
     * @brief Creates a matrix from its individual elements.
     *
     * Elements are specified in row-major order:
     *
     *     [ m00 m01 ]
     *     [ m10 m11 ]
     */
    Matrix2(float m00, float m01, float m10, float m11);

    /**
     * @brief Applies a scale transformation.
     *
     * @param scale X and Y scale factors.
     * @return The matrix multiplied by the scale matrix.
     */
    [[nodiscard]] Matrix2 Scale(const Vector2& scale) const;

    /**
     * @brief Applies a counter-clockwise rotation.
     *
     * @param radian Rotation angle in radians.
     * @return The matrix multiplied by the rotation matrix.
     */
    [[nodiscard]] Matrix2 Rotate(float radian) const;

    /**
     * @brief Returns the inverse of this matrix.
     *
     * If the matrix is not invertible, Identity is returned and a warning is sent
     */
    [[nodiscard]] Matrix2 Inverse() const;

    /**
     * @brief Calculates the determinant of this matrix.
     */
    [[nodiscard]] float Determinant() const;

    /**
     * @brief Returns the transpose of this matrix.
     */
    [[nodiscard]] Matrix2 Transpose() const;

    /**
     * @brief Compares two matrices using an absolute error tolerance.
     *
     * @param mat2 Matrix to compare against.
     * @param epsilon Maximum allowed difference between corresponding elements.
     */
    [[nodiscard]] bool NearlyEquals(const Matrix2& mat2, float epsilon = EPSILON) const;

    Matrix2 operator+(const Matrix2& mat2) const;
    Matrix2 operator-(const Matrix2& mat2) const;
    Matrix2 operator*(const Matrix2& mat2) const;

    Matrix2& operator+=(const Matrix2& mat2);
    Matrix2& operator-=(const Matrix2& mat2);
    Matrix2& operator*=(const Matrix2& mat2);

    /**
     * @brief Multiplies this matrix by a column vector.
     */
    Vector2 operator*(const Vector2& vec2) const;

    Matrix2 operator*(float scalar) const;
    Matrix2 operator/(float scalar) const;

    Matrix2& operator*=(float scalar);
    Matrix2& operator/=(float scalar);

    /**
     * @brief Returns the additive inverse of this matrix.
     */
    Matrix2 operator-() const;

    bool operator==(const Matrix2& mat2) const;
    bool operator!=(const Matrix2& mat2) const;

    /**
     * @brief Matrix containing only zeros.
     */
    static const Matrix2 Zero;

    /**
     * @brief 2x2 identity matrix.
     *
     *     [ 1 0 ]
     *     [ 0 1 ]
     */
    static const Matrix2 Identity;

    friend Matrix2 operator*(float scalar, const Matrix2& mat2);
    friend std::ostream& operator<<(std::ostream& os, const Matrix2& mat2);
};
} // namespace N::M
