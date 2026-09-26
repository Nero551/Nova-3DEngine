#pragma once

#include "BasicMatrix.hpp"

namespace N::M
{
template <unsigned int Row, unsigned int Column> struct Matrix : BasicMatrix<Row, Column, Matrix<Row, Column>>
{
    using BasicMatrix<Row, Column, Matrix>::BasicMatrix;
    using BasicMatrix<Row, Column, Matrix>::operator*;

    /** @brief Multiplies this matrix by another matrix. */
    template <unsigned int R, unsigned int C>
    constexpr Matrix<Row, C> operator*(const Matrix<R, C>& matrix) const requires(Column == R)
    {
        Matrix<Row, C> result{0};

        for (unsigned int row = 0; row < Row; ++row)
        {
            for (unsigned int column = 0; column < C; ++column)
            {
                for (unsigned int k = 0; k < R; ++k)
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

    /** @brief Returns the minor produced by removing a row and column. */
    constexpr Matrix<Row - 1, Column - 1> Minor(const unsigned int row, const unsigned int column) const
    {
        Matrix<Row - 1, Column - 1> result;
        unsigned int resultRow = 0;

        for (unsigned int currentRow = 0; currentRow < Row; ++currentRow)
        {
            if (currentRow == row)
            {
                continue;
            }

            unsigned int resultColumn = 0;

            for (unsigned int currentColumn = 0; currentColumn < Column; ++currentColumn)
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

    /** @brief Returns the determinant of the matrix. */
    constexpr float Determinant() const requires(Row == Column && Column > 1)
    {
        float result = 0;

        for (unsigned int column = 0; column < Column; ++column)
        {
            float cofactor = Minor(0, column).Determinant();

            if (column % 2 == 1)
            {
                cofactor = -cofactor;
            }

            result += (*this)(0, column) * cofactor;
        }

        return result;
    }

    constexpr Matrix Inverse() const requires(Row == Column && Column > 1)
    {
        const float determinant = Determinant();

        if (std::abs(determinant) < EPSILON)
        {
            U::Log::Error("Matrix is not invertible");
            return Matrix::Identity();
        }

        Matrix cofactorMatrix{0};

        for (unsigned int row = 0; row < Row; ++row)
        {
            for (unsigned int column = 0; column < Column; ++column)
            {
                float value = Minor(row, column).Determinant();

                if ((row + column) % 2 == 1)
                {
                    value = -value;
                }

                cofactorMatrix(row, column) = value;
            }
        }

        return cofactorMatrix.Transpose() / determinant;
    }
};
} // namespace N::M