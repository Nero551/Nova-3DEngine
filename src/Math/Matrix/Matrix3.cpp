#include "Matrix3.hpp"

#include "../Geometry/Basis.hpp"
#include "Math/Common/Comparison.hpp"
#include "Utilities/Logger.hpp"

namespace N::M
{
Matrix3::Matrix3(const float mAll)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            (*this)(row, col) = mAll;
        }
    }
}

Matrix3::Matrix3(const float m00, const float m01, const float m02, const float m10, const float m11,
    const float m12, const float m20, const float m21, const float m22)
{
    (*this)(0, 0) = m00;
    (*this)(0, 1) = m01;
    (*this)(0, 2) = m02;

    (*this)(1, 0) = m10;
    (*this)(1, 1) = m11;
    (*this)(1, 2) = m12;

    (*this)(2, 0) = m20;
    (*this)(2, 1) = m21;
    (*this)(2, 2) = m22;
}

//? Operations

Matrix3 Matrix3::Scale(const Vector3& scale) const
{
    Matrix3 result = *this;
    for (size_t row = 0; row < 3; ++row)
    {
        result(row, 0) *= scale.x;
        result(row, 1) *= scale.y;
        result(row, 2) *= scale.z;
    }

    return result;
}

Matrix3 Matrix3::RotateX(const float radian) const
{
    Matrix3 rotationMatrix = Identity;
    rotationMatrix(1, 1) = std::cos(radian);
    rotationMatrix(2, 1) = std::sin(radian);
    rotationMatrix(1, 2) = -std::sin(radian);
    rotationMatrix(2, 2) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix3 Matrix3::RotateY(const float radian) const
{
    Matrix3 rotationMatrix = Identity;
    rotationMatrix(0, 0) = std::cos(radian);
    rotationMatrix(0, 2) = std::sin(radian);
    rotationMatrix(2, 0) = -std::sin(radian);
    rotationMatrix(2, 2) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix3 Matrix3::RotateZ(const float radian) const
{
    Matrix3 rotationMatrix = Identity;
    rotationMatrix(0, 0) = std::cos(radian);
    rotationMatrix(1, 0) = std::sin(radian);
    rotationMatrix(0, 1) = -std::sin(radian);
    rotationMatrix(1, 1) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix3 Matrix3::Rotate(const Vector3& eulerRotation) const
{
    Matrix3 rotationMatrix = Identity;
    rotationMatrix = rotationMatrix.RotateZ(eulerRotation.z);
    rotationMatrix = rotationMatrix.RotateY(eulerRotation.y);
    rotationMatrix = rotationMatrix.RotateX(eulerRotation.x);

    return *this * rotationMatrix;
}

Matrix3 Matrix3::RotateAroundAxis(const Vector3& axis, const float radian) const
{
    Matrix3 rotationMatrix = Identity;
    rotationMatrix = rotationMatrix.RotateZ(radian);

    Vector3 forward = axis.Normalized();
    Vector3 helper = forward.IsParallelTo(Vector3::Up) ? Vector3::Right : Vector3::Up;

    Vector3 right = helper.Cross(forward);
    Vector3 up = forward.Cross(right);

    const Basis basis(right, up, forward);
    const Matrix3 basisMatrix = basis.GetMatrix().ToMatrix3();

    const Matrix3 finalMatrix = basisMatrix * rotationMatrix * basisMatrix.Inverse();

    return *this * finalMatrix;
}

Matrix3 Matrix3::Translate(const Vector2& trans) const
{
    Matrix3 transMatrix = Identity;

    transMatrix(0, 2) = trans.x;
    transMatrix(1, 2) = trans.y;

    return *this * transMatrix;
}

float Matrix3::Determinant() const
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

Matrix3 Matrix3::Transpose() const
{
    Matrix3 result;

    result(0, 0) = (*this)(0, 0);
    result(0, 1) = (*this)(1, 0);
    result(0, 2) = (*this)(2, 0);

    result(1, 0) = (*this)(0, 1);
    result(1, 1) = (*this)(1, 1);
    result(1, 2) = (*this)(2, 1);

    result(2, 0) = (*this)(0, 2);
    result(2, 1) = (*this)(1, 2);
    result(2, 2) = (*this)(2, 2);

    return result;
}

Matrix3 Matrix3::Inverse() const
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

    const float det = a * A + b * B + c * C;

    if (std::abs(det) < EPSILON)
    {
        U::Logger::Error("Matrix is not invertible");
        return Identity;
    }

    const float inverseDet = 1.0f / det;

    Matrix3 result;

    result(0, 0) = A * inverseDet;
    result(0, 1) = (c * h - b * i) * inverseDet;
    result(0, 2) = (b * f - c * e) * inverseDet;

    result(1, 0) = B * inverseDet;
    result(1, 1) = (a * i - c * g) * inverseDet;
    result(1, 2) = (c * d - a * f) * inverseDet;

    result(2, 0) = C * inverseDet;
    result(2, 1) = (b * g - a * h) * inverseDet;
    result(2, 2) = (a * e - b * d) * inverseDet;

    return result;
}

Matrix2 Matrix3::Minor(const int row, const int col) const
{
    Matrix2 minor;
    int minorRow = 0;

    for (int r = 0; r < 3; ++r)
    {
        if (r != row)
        {
            int minorCol = 0;

            for (int c = 0; c < 3; ++c)
            {
                if (c != col)
                {
                    minor(minorRow, minorCol) = (*this)(r, c);
                    ++minorCol;
                }
            }

            ++minorRow;
        }
    }

    return minor;
}

bool Matrix3::NearlyEquals(const Matrix3& mat3, const float epsilon) const
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (!M::NearlyEquals((*this)(row, col), mat3(row, col), epsilon))
            {
                return false;
            }
        }
    }

    return true;
}

Matrix4 Matrix3::ToMatrix4() const
{
    Matrix4 result{};

    result(0, 0) = (*this)(0, 0);
    result(0, 1) = (*this)(0, 1);
    result(0, 2) = (*this)(0, 2);

    result(1, 0) = (*this)(1, 0);
    result(1, 1) = (*this)(1, 1);
    result(1, 2) = (*this)(1, 2);

    result(2, 0) = (*this)(2, 0);
    result(2, 1) = (*this)(2, 1);
    result(2, 2) = (*this)(2, 2);

    return result;
}

float& Matrix3::operator()(const int row, const int col)
{
    return m[col][row];
}

const float& Matrix3::operator()(const int row, const int col) const
{
    return m[col][row];
}

//* Matrices
Matrix3 Matrix3::operator+(const Matrix3& mat3) const
{
    Matrix3 result = Zero;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            result(row, col) = (*this)(row, col) + mat3(row, col);
        }
    }

    return result;
}

Matrix3 Matrix3::operator-(const Matrix3& mat3) const
{
    Matrix3 result = Zero;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            result(row, col) = (*this)(row, col) - mat3(row, col);
        }
    }

    return result;
}

Matrix3 Matrix3::operator*(const Matrix3& mat3) const
{
    Matrix3 result(0);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            for (int k = 0; k < 3; ++k)
            {
                result(row, col) += (*this)(row, k) * mat3(k, col);
            }
        }
    }

    return result;
}

Matrix3& Matrix3::operator+=(const Matrix3& mat3)
{
    return *this = *this + mat3;
}

Matrix3& Matrix3::operator-=(const Matrix3& mat3)
{
    return *this = *this - mat3;
}

Matrix3& Matrix3::operator*=(const Matrix3& mat3)
{
    return *this = *this * mat3;
}

//* Vectors
Vector3 Matrix3::operator*(const Vector3& vec3) const
{
    return {(*this)(0, 0) * vec3.x + (*this)(0, 1) * vec3.y + (*this)(0, 2) * vec3.z,
        (*this)(1, 0) * vec3.x + (*this)(1, 1) * vec3.y + (*this)(1, 2) * vec3.z,
        (*this)(2, 0) * vec3.x + (*this)(2, 1) * vec3.y + (*this)(2, 2) * vec3.z};
}

//* Scalars
Matrix3 Matrix3::operator*(const float scalar) const
{
    Matrix3 result = Zero;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            result(row, col) = (*this)(row, col) * scalar;
        }
    }

    return result;
}

Matrix3 Matrix3::operator/(const float scalar) const
{
    Matrix3 result = Zero;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            result(row, col) = (*this)(row, col) / scalar;
        }
    }

    return result;
}

Matrix3& Matrix3::operator*=(const float scalar)
{
    return *this = *this * scalar;
}

Matrix3& Matrix3::operator/=(const float scalar)
{
    return *this = *this / scalar;
}

Matrix3 Matrix3::operator-() const
{
    return *this * -1;
}

//* Equality
bool Matrix3::operator==(const Matrix3& mat3) const
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if ((*this)(row, col) != mat3(row, col))
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix3::operator!=(const Matrix3& mat3) const
{
    return !(*this == mat3);
}

//? Statics
Matrix3 const Matrix3::Zero = Matrix3(0);

Matrix3 const Matrix3::Identity = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);

//* Others
Matrix3 operator*(const float scalar, const Matrix3& mat3)
{
    return mat3 * scalar;
}

std::ostream& operator<<(std::ostream& os, const Matrix3& mat3)
{
    os << "[ " << mat3(0, 0) << "  " << mat3(0, 1) << "  " << mat3(0, 2) << " ]\n";
    os << "[ " << mat3(1, 0) << "  " << mat3(1, 1) << "  " << mat3(1, 2) << " ]\n";
    os << "[ " << mat3(2, 0) << "  " << mat3(2, 1) << "  " << mat3(2, 2) << " ]";

    return os;
}
} // namespace N::M