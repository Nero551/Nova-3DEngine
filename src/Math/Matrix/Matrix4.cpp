#include "Matrix4.hpp"

#include "Math/Common/Comparison.hpp"
#include "Math/Geometry/Basis.hpp"
#include "Utilities/Logger.hpp"

namespace N::M
{
Matrix4::Matrix4(const float mAll)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            (*this)(row, col) = mAll;
        }
    }
}

Matrix4::Matrix4(const float m00, const float m01, const float m02, const float m03, const float m10,
    const float m11, const float m12, const float m13, const float m20, const float m21, const float m22,
    const float m23, const float m30, const float m31, const float m32, const float m33)
{
    (*this)(0, 0) = m00;
    (*this)(0, 1) = m01;
    (*this)(0, 2) = m02;
    (*this)(0, 3) = m03;

    (*this)(1, 0) = m10;
    (*this)(1, 1) = m11;
    (*this)(1, 2) = m12;
    (*this)(1, 3) = m13;

    (*this)(2, 0) = m20;
    (*this)(2, 1) = m21;
    (*this)(2, 2) = m22;
    (*this)(2, 3) = m23;

    (*this)(3, 0) = m30;
    (*this)(3, 1) = m31;
    (*this)(3, 2) = m32;
    (*this)(3, 3) = m33;
}

//? Operations

Matrix4 Matrix4::Translate(const Vector3& translation) const
{
    Matrix4 transMatrix = Identity;
    transMatrix(0, 3) = translation.x;
    transMatrix(1, 3) = translation.y;
    transMatrix(2, 3) = translation.z;

    return *this * transMatrix;
}

//? Methods
Matrix4 Matrix4::Scale(const Vector3& scale) const
{
    Matrix4 scaleMatrix = Identity;
    scaleMatrix(0, 0) = scale.x;
    scaleMatrix(1, 1) = scale.y;
    scaleMatrix(2, 2) = scale.z;

    return *this * scaleMatrix;
}

Matrix4 Matrix4::RotateX(const float radian) const
{
    Matrix4 rotationMatrix = Identity;
    rotationMatrix(1, 1) = std::cos(radian);
    rotationMatrix(2, 1) = std::sin(radian);
    rotationMatrix(1, 2) = -std::sin(radian);
    rotationMatrix(2, 2) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateY(const float radian) const
{
    Matrix4 rotationMatrix = Identity;
    rotationMatrix(0, 0) = std::cos(radian);
    rotationMatrix(0, 2) = std::sin(radian);
    rotationMatrix(2, 0) = -std::sin(radian);
    rotationMatrix(2, 2) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateZ(const float radian) const
{
    Matrix4 rotationMatrix = Identity;
    rotationMatrix(0, 0) = std::cos(radian);
    rotationMatrix(1, 0) = std::sin(radian);
    rotationMatrix(0, 1) = -std::sin(radian);
    rotationMatrix(1, 1) = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::Rotate(const Vector3& eulerRotation) const
{
    Matrix4 rotationMatrix = Identity;
    rotationMatrix = rotationMatrix.RotateZ(eulerRotation.z);
    rotationMatrix = rotationMatrix.RotateY(eulerRotation.y);
    rotationMatrix = rotationMatrix.RotateX(eulerRotation.x);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateAroundAxis(const Vector3& axis, const float radian) const
{
    Matrix4 rotationMatrix = Identity;
    rotationMatrix = rotationMatrix.RotateZ(radian);

    Vector3 forward = axis.Normalized();
    Vector3 helper = forward.IsParallelTo(Vector3::Up) ? Vector3::Right : Vector3::Up;

    Vector3 right = helper.Cross(forward);
    Vector3 up = forward.Cross(right);

    const Basis basis(right, up, forward);
    const Matrix4 basisMatrix = basis.GetMatrix();

    Matrix4 finalMatrix = basisMatrix * rotationMatrix * basisMatrix.Inverse();
    return *this * finalMatrix;
}

Matrix3 Matrix4::ToMatrix3() const
{
    return {(*this)(0, 0), (*this)(0, 1), (*this)(0, 2), (*this)(1, 0), (*this)(1, 1), (*this)(1, 2),
        (*this)(2, 0), (*this)(2, 1), (*this)(2, 2)};
}

Matrix4 Matrix4::Orthographic(const float left, const float right, const float bottom, const float top,
    const float near, const float far)
{
    Matrix4 matrix = Identity;

    matrix(0, 0) = 2.0f / (right - left);
    matrix(1, 1) = 2.0f / (top - bottom);
    matrix(2, 2) = -2.0f / (far - near);

    matrix(0, 3) = -(right + left) / (right - left);
    matrix(1, 3) = -(top + bottom) / (top - bottom);
    matrix(2, 3) = -(far + near) / (far - near);

    return matrix;
}

Matrix4 Matrix4::Perspective(const float fovRad, const float aspectRatio, const float near, const float far)
{
    Matrix4 matrix = Zero;

    float f = 1.0f / std::tan(fovRad * 0.5f);

    matrix(0, 0) = f / aspectRatio;
    matrix(1, 1) = f;

    matrix(2, 2) = -(far + near) / (far - near);
    matrix(2, 3) = -(2.0f * far * near) / (far - near);

    matrix(3, 2) = -1.0f;

    return matrix;
}

Matrix4 Matrix4::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    Matrix4 trans = Identity;
    trans = trans.Translate(-pos);

    Vector3 forward = (target - pos).Normalized();
    Vector3 right = forward.Cross(up).Normalized();
    Vector3 upCorrect = right.Cross(forward);

    Basis basis(right, upCorrect, -forward);
    return basis.GetInverseMatrix() * trans;
}

float Matrix4::Determinant() const
{
    return (*this)(0, 0) * Minor(0, 0).Determinant() - (*this)(0, 1) * Minor(0, 1).Determinant() +
        (*this)(0, 2) * Minor(0, 2).Determinant() - (*this)(0, 3) * Minor(0, 3).Determinant();
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result(row, col) = (*this)(col, row);
        }
    }

    return result;
}

Matrix4 Matrix4::Inverse() const
{
    Matrix4 cofactorMatrix;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            Matrix3 minor = Minor(row, col);

            float det = minor.Determinant();

            if ((row + col) % 2 == 1)
            {
                det = -det;
            }

            cofactorMatrix(row, col) = det;
        }
    }

    float det = Determinant();

    if (M::NearlyEquals(std::abs(det), 0.0))
    {
        U::Logger::Error("Matrix is not invertible");
        return Identity;
    }

    return cofactorMatrix.Transpose() / det;
}

Matrix3 Matrix4::Minor(const int row, const int col) const
{
    Matrix3 minor;
    int minorRow = 0;

    for (int r = 0; r < 4; ++r)
    {
        if (r != row)
        {
            int minorCol = 0;

            for (int c = 0; c < 4; ++c)
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

bool Matrix4::NearlyEquals(const Matrix4& mat4, const float epsilon) const
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (!M::NearlyEquals((*this)(row, col), mat4(row, col), epsilon))
            {
                return false;
            }
        }
    }

    return true;
}

float& Matrix4::operator()(const int row, const int col)
{
    return m[col][row];
}

const float& Matrix4::operator()(const int row, const int col) const
{
    return m[col][row];
}

//* Matrices
Matrix4 Matrix4::operator+(const Matrix4& mat4) const
{
    Matrix4 result = Zero;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result(row, col) = (*this)(row, col) + mat4(row, col);
        }
    }

    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& mat4) const
{
    Matrix4 result = Zero;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result(row, col) = (*this)(row, col) - mat4(row, col);
        }
    }

    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& mat4) const
{
    Matrix4 result = Zero;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            for (int k = 0; k < 4; ++k)
            {
                result(row, col) += (*this)(row, k) * mat4(k, col);
            }
        }
    }

    return result;
}

Matrix4& Matrix4::operator+=(const Matrix4& mat4)
{
    return *this = *this + mat4;
}

Matrix4& Matrix4::operator-=(const Matrix4& mat4)
{
    return *this = *this - mat4;
}

Matrix4& Matrix4::operator*=(const Matrix4& mat4)
{
    return *this = *this * mat4;
}

//* Vectors
Vector4 Matrix4::operator*(const Vector4& vec4) const
{
    return {(*this)(0, 0) * vec4.x + (*this)(0, 1) * vec4.y + (*this)(0, 2) * vec4.z + (*this)(0, 3) * vec4.w,

        (*this)(1, 0) * vec4.x + (*this)(1, 1) * vec4.y + (*this)(1, 2) * vec4.z + (*this)(1, 3) * vec4.w,

        (*this)(2, 0) * vec4.x + (*this)(2, 1) * vec4.y + (*this)(2, 2) * vec4.z + (*this)(2, 3) * vec4.w,

        (*this)(3, 0) * vec4.x + (*this)(3, 1) * vec4.y + (*this)(3, 2) * vec4.z + (*this)(3, 3) * vec4.w};
}

//* Scalars
Matrix4 Matrix4::operator*(const float scalar) const
{
    Matrix4 result = Zero;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result(row, col) = (*this)(row, col) * scalar;
        }
    }

    return result;
}

Matrix4 Matrix4::operator/(const float scalar) const
{
    Matrix4 result = Zero;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            result(row, col) = (*this)(row, col) / scalar;
        }
    }

    return result;
}

Matrix4& Matrix4::operator*=(const float scalar)
{
    return *this = *this * scalar;
}

Matrix4& Matrix4::operator/=(const float scalar)
{
    return *this = *this / scalar;
}

Matrix4 Matrix4::operator-() const
{
    return *this * -1;
}

//* Equality
bool Matrix4::operator==(const Matrix4& mat4) const
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if ((*this)(row, col) != mat4(row, col))
            {
                return false;
            }
        }
    }

    return true;
}

bool Matrix4::operator!=(const Matrix4& mat4) const
{
    return !(*this == mat4);
}

//? Statics
Matrix4 const Matrix4::Zero = Matrix4(0);

Matrix4 const Matrix4::Identity = Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

//* Others
Matrix4 operator*(const float scalar, const Matrix4& mat4)
{
    return mat4 * scalar;
}

std::ostream& operator<<(std::ostream& os, const Matrix4& mat4)
{
    os << "[ " << mat4(0, 0) << "  " << mat4(0, 1) << "  " << mat4(0, 2) << "  " << mat4(0, 3) << " ]\n";

    os << "[ " << mat4(1, 0) << "  " << mat4(1, 1) << "  " << mat4(1, 2) << "  " << mat4(1, 3) << " ]\n";

    os << "[ " << mat4(2, 0) << "  " << mat4(2, 1) << "  " << mat4(2, 2) << "  " << mat4(2, 3) << " ]\n";

    os << "[ " << mat4(3, 0) << "  " << mat4(3, 1) << "  " << mat4(3, 2) << "  " << mat4(3, 3) << " ]";

    return os;
}
} // namespace N::M