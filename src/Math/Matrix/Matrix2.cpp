#include "Matrix2.hpp"

#include "Math/Common/Comparison.hpp"
#include "Utilities/Log.hpp"

namespace N::M
{
Matrix2::Matrix2(const float mAll)
{
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            (*this)(row, col) = mAll;
        }
    }
}

Matrix2::Matrix2(const float m00, const float m01, const float m10, const float m11)
{
    (*this)(0, 0) = m00;
    (*this)(0, 1) = m01;
    (*this)(1, 0) = m10;
    (*this)(1, 1) = m11;
}

//? Operations

//? Methods
Matrix2 Matrix2::Scale(const Vector<2>& scale) const
{
    Matrix2 scaleMatrix = Identity;
    scaleMatrix(0, 0) = scale.x;
    scaleMatrix(1, 1) = scale.y;

    return *this * scaleMatrix;
}

Matrix2 Matrix2::Rotate(const float radian) const
{
    Matrix2 rotationMatrix = Identity;

    rotationMatrix(0, 0) = std::cos(radian);
    rotationMatrix(1, 0) = std::sin(radian);
    rotationMatrix(0, 1) = -std::sin(radian);
    rotationMatrix(1, 1) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix2 Matrix2::Inverse() const
{
    float det = Determinant();

    if (std::abs(det) < EPSILON)
    {
        U::Log::Error("Matrix is not invertible");
        return Identity;
    }

    return Matrix2((*this)(1, 1), -(*this)(0, 1), -(*this)(1, 0), (*this)(0, 0)) / det;
}

float Matrix2::Determinant() const
{
    return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
}

Matrix2 Matrix2::Transpose() const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            result(row, col) = (*this)(col, row);
        }
    }

    return result;
}

bool Matrix2::NearlyEquals(const Matrix2& mat2, const float epsilon) const
{
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            if (!M::NearlyEquals((*this)(row, col), mat2(row, col), epsilon))
            {
                return false;
            }
        }
    }

    return true;
}

float& Matrix2::operator()(const int row, const int col)
{
    return m_Data[col][row];
}

const float& Matrix2::operator()(const int row, const int col) const
{
    return m_Data[col][row];
}

//* Matrices
Matrix2 Matrix2::operator+(const Matrix2& mat2) const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            result(row, col) = (*this)(row, col) + mat2(row, col);
        }
    }

    return result;
}

Matrix2 Matrix2::operator-(const Matrix2& mat2) const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            result(row, col) = (*this)(row, col) - mat2(row, col);
        }
    }

    return result;
}

Matrix2 Matrix2::operator*(const Matrix2& mat2) const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            for (int k = 0; k < 2; ++k)
            {
                result(row, col) += (*this)(row, k) * mat2(k, col);
            }
        }
    }

    return result;
}

Matrix2& Matrix2::operator+=(const Matrix2& mat2)
{
    return *this = *this + mat2;
}

Matrix2& Matrix2::operator-=(const Matrix2& mat2)
{
    return *this = *this - mat2;
}

Matrix2& Matrix2::operator*=(const Matrix2& mat2)
{
    return *this = *this * mat2;
}

//* Vectors
Vector<2> Matrix2::operator*(const Vector<2>& vec2) const
{
    return {(*this)(0, 0) * vec2.x + (*this)(0, 1) * vec2.y, (*this)(1, 0) * vec2.x + (*this)(1, 1) * vec2.y};
}

//* Scalars
Matrix2 Matrix2::operator*(const float scalar) const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            result(row, col) = (*this)(row, col) * scalar;
        }
    }

    return result;
}

Matrix2 Matrix2::operator/(const float scalar) const
{
    Matrix2 result = Zero;

    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            result(row, col) = (*this)(row, col) / scalar;
        }
    }

    return result;
}

Matrix2& Matrix2::operator*=(const float scalar)
{
    return *this = *this * scalar;
}

Matrix2& Matrix2::operator/=(const float scalar)
{
    return *this = *this / scalar;
}

Matrix2 Matrix2::operator-() const
{
    return *this * -1;
}

//* Equality
bool Matrix2::operator==(const Matrix2& mat2) const
{
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 2; ++col)
        {
            if ((*this)(row, col) != mat2(row, col))
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix2::operator!=(const Matrix2& mat2) const
{
    return !(*this == mat2);
}

//? Statics
Matrix2 const Matrix2::Zero = Matrix2(0);

Matrix2 const Matrix2::Identity = Matrix2(1, 0, 0, 1);

Matrix2 operator*(const float scalar, const Matrix2& mat2)
{
    return mat2 * scalar;
}

//* Others
std::ostream& operator<<(std::ostream& os, const Matrix2& mat2)
{
    os << "[ " << mat2(0, 0) << "  " << mat2(0, 1) << " ]\n";
    os << "[ " << mat2(1, 0) << "  " << mat2(1, 1) << " ]";

    return os;
}

} // namespace N::M