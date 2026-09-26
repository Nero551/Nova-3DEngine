#pragma once

#include "Math/Matrix/Matrix.hpp"
#include "Math/Matrix/Matrix3.hpp"
#include "Math/Vector/Vector4.hpp"
#include "Utilities/Log.hpp"

namespace N::M
{
template <> struct Matrix<4, 4> : BasicMatrix<4, 4, Matrix<4, 4>>
{
    using BasicMatrix::BasicMatrix;
    using BasicMatrix::operator*;

    /** @brief Applies a 3D translation. */
    constexpr Matrix Translate(const Vector<3>& translation) const
    {
        Matrix result = *this;

        result(0, 3) += translation(0);
        result(1, 3) += translation(1);
        result(2, 3) += translation(2);

        return result;
    }

    /** @brief Applies a 3D scale. */
    constexpr Matrix Scale(const Vector<3>& scale) const
    {
        Matrix result = *this;

        for (unsigned int row = 0; row < 3; ++row)
        {
            result(row, 0) *= scale(0);
            result(row, 1) *= scale(1);
            result(row, 2) *= scale(2);
        }

        return result;
    }

    /** @brief Applies a rotation around the X axis. */
    constexpr Matrix RotateX(const float radian) const
    {
        Matrix rotationMatrix = Identity();

        rotationMatrix(1, 1) = std::cos(radian);
        rotationMatrix(2, 1) = std::sin(radian);
        rotationMatrix(1, 2) = -std::sin(radian);
        rotationMatrix(2, 2) = std::cos(radian);

        return *this * rotationMatrix;
    }

    /** @brief Applies a rotation around the Y axis. */
    constexpr Matrix RotateY(const float radian) const
    {
        Matrix rotationMatrix = Identity();

        rotationMatrix(0, 0) = std::cos(radian);
        rotationMatrix(0, 2) = std::sin(radian);
        rotationMatrix(2, 0) = -std::sin(radian);
        rotationMatrix(2, 2) = std::cos(radian);

        return *this * rotationMatrix;
    }

    /** @brief Applies a rotation around the Z axis. */
    constexpr Matrix RotateZ(const float radian) const
    {
        Matrix rotationMatrix = Identity();

        rotationMatrix(0, 0) = std::cos(radian);
        rotationMatrix(1, 0) = std::sin(radian);
        rotationMatrix(0, 1) = -std::sin(radian);
        rotationMatrix(1, 1) = std::cos(radian);

        return *this * rotationMatrix;
    }

    /** @brief Applies Euler rotations in XYZ order. */
    constexpr Matrix Rotate(const Vector<3>& eulerRotation) const
    {
        Matrix rotationMatrix = Identity();

        rotationMatrix = rotationMatrix.RotateZ(eulerRotation(2));
        rotationMatrix = rotationMatrix.RotateY(eulerRotation(1));
        rotationMatrix = rotationMatrix.RotateX(eulerRotation(0));

        return *this * rotationMatrix;
    }

    /** @brief Applies a rotation around an arbitrary axis. */
    constexpr Matrix RotateAroundAxis(const Vector<3>& axis, const float radian) const
    {
        const Vector<3> forward = axis.Normalized();

        const float cosine = std::cos(radian);
        const float sine = std::sin(radian);
        const float oneMinusCosine = 1.0f - cosine;

        const float x = forward(0);
        const float y = forward(1);
        const float z = forward(2);

        Matrix rotationMatrix = Identity();

        rotationMatrix(0, 0) = oneMinusCosine * x * x + cosine;
        rotationMatrix(0, 1) = oneMinusCosine * x * y - sine * z;
        rotationMatrix(0, 2) = oneMinusCosine * x * z + sine * y;

        rotationMatrix(1, 0) = oneMinusCosine * x * y + sine * z;
        rotationMatrix(1, 1) = oneMinusCosine * y * y + cosine;
        rotationMatrix(1, 2) = oneMinusCosine * y * z - sine * x;

        rotationMatrix(2, 0) = oneMinusCosine * x * z - sine * y;
        rotationMatrix(2, 1) = oneMinusCosine * y * z + sine * x;
        rotationMatrix(2, 2) = oneMinusCosine * z * z + cosine;

        return *this * rotationMatrix;
    }

    /** @brief Extracts the upper-left 3x3 portion of the matrix. */
    constexpr Matrix<3, 3> ToMatrix3() const
    {
        return {(*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(1, 0), (*this)(1, 1), (*this)(1, 2),
            (*this)(2, 0), (*this)(2, 1), (*this)(2, 2)};
    }

    /** @brief Creates an orthographic projection matrix. */
    static constexpr Matrix Orthographic(const float left, const float right, const float bottom,
        const float top, const float near, const float far)
    {
        Matrix matrix = Identity();

        matrix(0, 0) = 2.0f / (right - left);
        matrix(1, 1) = 2.0f / (top - bottom);
        matrix(2, 2) = -2.0f / (far - near);

        matrix(0, 3) = -(right + left) / (right - left);
        matrix(1, 3) = -(top + bottom) / (top - bottom);
        matrix(2, 3) = -(far + near) / (far - near);

        return matrix;
    }

    /** @brief Creates a perspective projection matrix. */
    static constexpr Matrix Perspective(
        const float fovRad, const float aspectRatio, const float near, const float far)
    {
        Matrix matrix = Zero();

        const float f = 1.0f / std::tan(fovRad * 0.5f);

        matrix(0, 0) = f / aspectRatio;
        matrix(1, 1) = f;

        matrix(2, 2) = -(far + near) / (far - near);
        matrix(2, 3) = -(2.0f * far * near) / (far - near);

        matrix(3, 2) = -1.0f;

        return matrix;
    }

    /** @brief Creates a view matrix looking from one position toward another. */
    static constexpr Matrix LookAt(const Vector<3>& position, const Vector<3>& target, const Vector<3>& up)
    {
        Matrix translation = Identity();
        translation = translation.Translate(-position);

        const Vector<3> forward = (target - position).Normalized();
        const Vector<3> right = forward.Cross(up).Normalized();
        const Vector<3> correctedUp = right.Cross(forward);

        Matrix basisMatrix = Identity();

        basisMatrix(0, 0) = right.x;
        basisMatrix(1, 0) = right.y;
        basisMatrix(2, 0) = right.z;

        basisMatrix(0, 1) = correctedUp.x;
        basisMatrix(1, 1) = correctedUp.y;
        basisMatrix(2, 1) = correctedUp.z;

        basisMatrix(0, 2) = -forward.x;
        basisMatrix(1, 2) = -forward.y;
        basisMatrix(2, 2) = -forward.z;

        return basisMatrix.Inverse() * translation;
    }

    /** @brief Returns the determinant of the matrix. */
    constexpr float Determinant() const
    {
        return (*this)(0, 0) * Minor(0, 0).Determinant() - (*this)(0, 1) * Minor(0, 1).Determinant() +
            (*this)(0, 2) * Minor(0, 2).Determinant() - (*this)(0, 3) * Minor(0, 3).Determinant();
    }

    /** @brief Returns the inverse of the matrix. */
    constexpr Matrix Inverse() const
    {
        Matrix cofactorMatrix = Zero();

        for (unsigned int row = 0; row < 4; ++row)
        {
            for (unsigned int column = 0; column < 4; ++column)
            {
                float determinant = Minor(row, column).Determinant();

                if ((row + column) % 2 == 1)
                {
                    determinant = -determinant;
                }

                cofactorMatrix(row, column) = determinant;
            }
        }

        const float determinant = Determinant();

        if (M::NearlyEquals(std::abs(determinant), 0.0f))
        {
            U::Log::Error("Matrix is not invertible");
            return Identity();
        }

        return cofactorMatrix.Transpose() / determinant;
    }

    /** @brief Returns the minor produced by removing a row and column. */
    constexpr Matrix<3, 3> Minor(const unsigned int row, const unsigned int column) const
    {
        Matrix<3, 3> result;
        unsigned int resultRow = 0;

        for (unsigned int currentRow = 0; currentRow < 4; ++currentRow)
        {
            if (currentRow == row)
            {
                continue;
            }

            unsigned int resultColumn = 0;

            for (unsigned int currentColumn = 0; currentColumn < 4; ++currentColumn)
            {
                if (currentColumn == column)
                {
                    continue;
                }

                result(resultRow, resultColumn) = (*this)(currentRow, currentColumn);

                ++resultColumn;
            }

            ++resultRow;
        }

        return result;
    }

    Matrix operator*(const Matrix& mat4) const
    {
        Matrix result;

        for (int row = 0; row < 4; ++row)
        {
            const float a0 = (*this)(row, 0);
            const float a1 = (*this)(row, 1);
            const float a2 = (*this)(row, 2);
            const float a3 = (*this)(row, 3);

            result(row, 0) = a0 * mat4(0, 0) + a1 * mat4(1, 0) + a2 * mat4(2, 0) + a3 * mat4(3, 0);

            result(row, 1) = a0 * mat4(0, 1) + a1 * mat4(1, 1) + a2 * mat4(2, 1) + a3 * mat4(3, 1);

            result(row, 2) = a0 * mat4(0, 2) + a1 * mat4(1, 2) + a2 * mat4(2, 2) + a3 * mat4(3, 2);

            result(row, 3) = a0 * mat4(0, 3) + a1 * mat4(1, 3) + a2 * mat4(2, 3) + a3 * mat4(3, 3);
        }

        return result;
    }

    /** @brief Multiplies this matrix by another matrix. */
    constexpr Matrix operator*=(const Matrix& matrix)
    {
        return *this = *this * matrix;
    }
};
} // namespace N::M