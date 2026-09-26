#pragma once

#include "Math/Matrix/Matrix.hpp"
#include "Math/Matrix/Matrix2.hpp"
#include "Math/Vector/Vector3.hpp"

#include "Utilities/Log.hpp"

namespace N::M
{
template <> struct Matrix<3, 3> : BasicMatrix<3, 3, Matrix<3, 3>>
{
    using BasicMatrix::BasicMatrix;
    using BasicMatrix::operator*;

    /** @brief Applies a scale transformation. */
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

    /** @brief Applies a 2D translation using homogeneous coordinates. */
    constexpr Matrix Translate(const Vector<2>& translation) const
    {
        Matrix translationMatrix = Identity();

        translationMatrix(0, 2) = translation(0);
        translationMatrix(1, 2) = translation(1);

        return *this * translationMatrix;
    }

    /** @brief Returns the determinant of the matrix. */
    constexpr float Determinant() const
    {
        const float a = (*this)(0, 0);
        const float b = (*this)(0, 1);
        const float c = (*this)(0, 2);
        const float d = (*this)(1, 0);
        const float e = (*this)(1, 1);
        const float f = (*this)(1, 2);
        const float g = (*this)(2, 0);
        const float h = (*this)(2, 1);
        const float i = (*this)(2, 2);

        return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    }

    /** @brief Returns the inverse of the matrix. */
    constexpr Matrix Inverse() const
    {
        const float a = (*this)(0, 0);
        const float b = (*this)(0, 1);
        const float c = (*this)(0, 2);
        const float d = (*this)(1, 0);
        const float e = (*this)(1, 1);
        const float f = (*this)(1, 2);
        const float g = (*this)(2, 0);
        const float h = (*this)(2, 1);
        const float i = (*this)(2, 2);

        const float A = e * i - f * h;
        const float B = f * g - d * i;
        const float C = d * h - e * g;

        const float determinant = a * A + b * B + c * C;

        if (std::abs(determinant) < EPSILON)
        {
            U::Log::Error("Matrix is not invertible");
            return Identity();
        }

        const float inverseDeterminant = 1.0f / determinant;

        Matrix result;

        result(0, 0) = A * inverseDeterminant;
        result(0, 1) = (c * h - b * i) * inverseDeterminant;
        result(0, 2) = (b * f - c * e) * inverseDeterminant;

        result(1, 0) = B * inverseDeterminant;
        result(1, 1) = (a * i - c * g) * inverseDeterminant;
        result(1, 2) = (c * d - a * f) * inverseDeterminant;

        result(2, 0) = C * inverseDeterminant;
        result(2, 1) = (b * g - a * h) * inverseDeterminant;
        result(2, 2) = (a * e - b * d) * inverseDeterminant;

        return result;
    }

    /** @brief Returns the minor produced by removing a row and column. */
    constexpr Matrix<2, 2> Minor(const unsigned int row, const unsigned int column) const
    {
        Matrix<2, 2> result;
        unsigned int resultRow = 0;

        for (unsigned int currentRow = 0; currentRow < 3; ++currentRow)
        {
            if (currentRow == row)
            {
                continue;
            }

            unsigned int resultColumn = 0;

            for (unsigned int currentColumn = 0; currentColumn < 3; ++currentColumn)
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

    /** @brief Multiplies this matrix by another matrix. */
    constexpr Matrix operator*(const Matrix& matrix) const
    {
        Matrix result{0};

        for (unsigned int row = 0; row < 3; ++row)
        {
            for (unsigned int column = 0; column < 3; ++column)
            {
                for (unsigned int k = 0; k < 3; ++k)
                {
                    result(row, column) += (*this)(row, k) * matrix(k, column);
                }
            }
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