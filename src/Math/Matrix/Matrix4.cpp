#include "Matrix4.hpp"

#include "Math/Common/Comparison.hpp"
#include "Math/Geometry/Basis.hpp"
#include "Utilities/Logger.hpp"

namespace E::M {
Matrix4::Matrix4(const float mAll) {
    for (auto& row : m) {
        for (float& col : row) {
            col = mAll;
        }
    }
}

Matrix4::Matrix4(const float m00, const float m01, const float m02, const float m03, const float m10, const float m11,
    const float m12, const float m13, const float m20, const float m21, const float m22, const float m23, const float m30,
    const float m31, const float m32, const float m33) {
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;

    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;

    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;

    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

//? Operations

Matrix4 Matrix4::Translate(const Vector3& translation) const {
    Matrix4 transMatrix = Identity;
    transMatrix.m[0][3] = translation.x;
    transMatrix.m[1][3] = translation.y;
    transMatrix.m[2][3] = translation.z;

    return *this * transMatrix;
}

//? Methods
Matrix4 Matrix4::Scale(const Vector3& scale) const {
    Matrix4 scaleMatrix = Identity;
    scaleMatrix.m[0][0] = scale.x;
    scaleMatrix.m[1][1] = scale.y;
    scaleMatrix.m[2][2] = scale.z;

    return *this * scaleMatrix;
}

Matrix4 Matrix4::RotateX(const float radian) const {
    Matrix4 rotationMatrix = Identity;
    rotationMatrix.m[1][1] = std::cos(radian);
    rotationMatrix.m[2][1] = std::sin(radian);
    rotationMatrix.m[1][2] = -std::sin(radian);
    rotationMatrix.m[2][2] = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateY(const float radian) const {
    Matrix4 rotationMatrix = Identity;
    rotationMatrix.m[0][0] = std::cos(radian);
    rotationMatrix.m[0][2] = std::sin(radian);
    rotationMatrix.m[2][0] = -std::sin(radian);
    rotationMatrix.m[2][2] = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateZ(const float radian) const {
    Matrix4 rotationMatrix = Identity;
    rotationMatrix.m[0][0] = std::cos(radian);
    rotationMatrix.m[1][0] = std::sin(radian);
    rotationMatrix.m[0][1] = -std::sin(radian);
    rotationMatrix.m[1][1] = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::Rotate(const Vector3& eulerRotation) const {
    Matrix4 rotationMatrix = Identity;
    rotationMatrix = rotationMatrix.RotateZ(eulerRotation.z);
    rotationMatrix = rotationMatrix.RotateY(eulerRotation.y);
    rotationMatrix = rotationMatrix.RotateX(eulerRotation.x);

    return *this * rotationMatrix;
}

Matrix4 Matrix4::RotateAroundAxis(const Vector3& axis, const float radian) const {
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

Matrix3 Matrix4::ToMatrix3() const {
    return { m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2] };
}

float Matrix4::Determinant() const {
    return m[0][0] * Minor(0, 0).Determinant() - m[0][1] * Minor(0, 1).Determinant() + m[0][2] * Minor(0, 2).Determinant() -
        m[0][3] * Minor(0, 3).Determinant();
}

Matrix4 Matrix4::Transpose() const {
    Matrix4 result;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m[col][row];
        }
    }
    return result;
}

Matrix4 Matrix4::Inverse() const {
    Matrix4 cofactorMatrix;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            Matrix3 minor = Minor(row, col);

            float det = minor.Determinant();

            if ((row + col) % 2 == 1) {
                det = -det;
            }

            cofactorMatrix.m[row][col] = det;
        }
    }

    float det = Determinant();

    if (M::NearlyEquals(std::abs(det), 0.0)) {
        U::Logger::Error("Matrix is not invertible");
        return Identity;
    }

    return cofactorMatrix.Transpose() / det;
}

Matrix3 Matrix4::Minor(const int row, const int col) const {
    Matrix3 minor;
    int minorRow = 0;

    for (int r = 0; r < 4; r++) {
        if (r != row) {
            int minorCol = 0;

            for (int c = 0; c < 4; c++) {
                if (c != col) {
                    minor.m[minorRow][minorCol] = m[r][c];
                    minorCol++;
                }
            }
            minorRow++;
        }
    }

    return minor;
}

bool Matrix4::NearlyEquals(const Matrix4& mat4, const float epsilon) const {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (!M::NearlyEquals(m[row][col], mat4.m[row][col], epsilon)) {
                return false;
            }
        }
    }
    return true;
}

//* Matrices
Matrix4 Matrix4::operator+(const Matrix4& mat4) const {
    Matrix4 result = Zero;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m[row][col] + mat4.m[row][col];
        }
    }
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& mat4) const {
    Matrix4 result = Zero;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m[row][col] - mat4.m[row][col];
        }
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& mat4) const {
    Matrix4 result = Zero;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result.m[row][col] += m[row][k] * mat4.m[k][col];
            }
        }
    }

    return result;
}

Matrix4& Matrix4::operator+=(const Matrix4& mat4) {
    return *this = *this + mat4;
}

Matrix4& Matrix4::operator-=(const Matrix4& mat4) {
    return *this = *this - mat4;
}

Matrix4& Matrix4::operator*=(const Matrix4& mat4) {
    return *this = *this * mat4;
}

//* Vectors
Vector4 Matrix4::operator*(const Vector4& vec4) const {
    return { m[0][0] * vec4.x + m[0][1] * vec4.y + m[0][2] * vec4.z + m[0][3] * vec4.w,
        m[1][0] * vec4.x + m[1][1] * vec4.y + m[1][2] * vec4.z + m[1][3] * vec4.w,
        m[2][0] * vec4.x + m[2][1] * vec4.y + m[2][2] * vec4.z + m[2][3] * vec4.w,
        m[3][0] * vec4.x + m[3][1] * vec4.y + m[3][2] * vec4.z + m[3][3] * vec4.w };
}

//* Scalars
Matrix4 Matrix4::operator*(const float scalar) const {
    Matrix4 result = Zero;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m[row][col] * scalar;
        }
    }

    return result;
}

Matrix4 Matrix4::operator/(const float scalar) const {
    Matrix4 result = Zero;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = m[row][col] / scalar;
        }
    }

    return result;
}

Matrix4& Matrix4::operator*=(const float scalar) {
    return *this = *this * scalar;
}

Matrix4& Matrix4::operator/=(const float scalar) {
    return *this = *this / scalar;
}

Matrix4 Matrix4::operator-() const {
    return *this * -1;
}

//* Equality
bool Matrix4::operator==(const Matrix4& mat4) const {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (m[row][col] != mat4.m[row][col])
                return false;
        }
    }

    return true;
}

bool Matrix4::operator!=(const Matrix4& mat4) const {
    return !(*this == mat4);
}

Matrix4 Matrix4::Orthographic(
    const float left, const float right, const float bottom, const float top, const float near, const float far) {
    Matrix4 m = Identity;

    m.m[0][0] = 2.0f / (right - left);
    m.m[1][1] = 2.0f / (top - bottom);
    m.m[2][2] = -2.0f / (far - near);

    m.m[0][3] = -(right + left) / (right - left);
    m.m[1][3] = -(top + bottom) / (top - bottom);
    m.m[2][3] = -(far + near) / (far - near);

    return m;
}

Matrix4 Matrix4::Perspective(const float fovRad, const float aspectRatio, const float near, const float far) {
    Matrix4 m = Zero;

    float f = 1.0f / std::tan(fovRad * 0.5f);

    m.m[0][0] = f / aspectRatio;
    m.m[1][1] = f;

    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -(2.0f * far * near) / (far - near);

    m.m[3][2] = -1.0f;

    return m;
}

Matrix4 Matrix4::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up) {
    Matrix4 trans = Identity;
    trans = trans.Translate(-pos);

    Vector3 forward = (target - pos).Normalized();
    Vector3 right = up.Cross(forward).Normalized();
    Vector3 upCorrect = forward.Cross(right);

    Basis basis(right, upCorrect, -forward);
    return basis.GetInverseMatrix() * trans;
}

//? Statics
Matrix4 const Matrix4::Zero = Matrix4(0);

Matrix4 const Matrix4::Identity = Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

//* Others
Matrix4 operator*(const float scalar, const Matrix4& mat4) {
    return mat4 * scalar;
}

std::ostream& operator<<(std::ostream& os, const Matrix4& mat4) {
    os << "[ " << mat4.m[0][0] << "  " << mat4.m[0][1] << "  " << mat4.m[0][2] << "  " << mat4.m[0][3] << " ]\n";
    os << "[ " << mat4.m[1][0] << "  " << mat4.m[1][1] << "  " << mat4.m[1][2] << "  " << mat4.m[1][3] << " ]\n";
    os << "[ " << mat4.m[2][0] << "  " << mat4.m[2][1] << "  " << mat4.m[2][2] << "  " << mat4.m[2][3] << " ]\n";
    os << "[ " << mat4.m[3][0] << "  " << mat4.m[3][1] << "  " << mat4.m[3][2] << "  " << mat4.m[3][3] << " ]";
    return os;
}
} // namespace E::M
