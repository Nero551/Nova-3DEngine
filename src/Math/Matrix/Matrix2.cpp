#include "Matrix2.hpp"

#include "Math/Common/Comparison.hpp"
#include "Utilities/Logger.hpp"

namespace N::M {
Matrix2::Matrix2(const float mAll) {
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            m[row][col] = mAll;
        }
    }
}

Matrix2::Matrix2(const float m00, const float m01, const float m10, const float m11) {
    m[0][0] = m00;
    m[0][1] = m01;
    m[1][0] = m10;
    m[1][1] = m11;
}

//? Operations

//? Methods
Matrix2 Matrix2::Scale(const Vector2& scale) const {
    Matrix2 scaleMatrix = Identity;
    scaleMatrix.m[0][0] = scale.x;
    scaleMatrix.m[1][1] = scale.y;

    return *this * scaleMatrix;
}

Matrix2 Matrix2::Rotate(const float radian) const {
    Matrix2 rotationMatrix = Identity;
    rotationMatrix.m[0][0] = std::cos(radian);
    rotationMatrix.m[1][0] = std::sin(radian);
    rotationMatrix.m[0][1] = -std::sin(radian);
    rotationMatrix.m[1][1] = std::cos(radian);

    return *this * rotationMatrix;
}

Matrix2 Matrix2::Inverse() const {
    float det = Determinant();
    if (std::abs(det) < EPSILON) {
        U::Logger::Error("Matrix is not invertible");
        return Identity;
    }
    return Matrix2(m[1][1], -m[0][1], -m[1][0], m[0][0]) / det;
}

float Matrix2::Determinant() const {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Matrix2 Matrix2::Transpose() const {
    Matrix2 result = Zero;

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            result.m[row][col] = m[col][row];
        }
    }

    return result;
}

bool Matrix2::NearlyEquals(const Matrix2& mat2, const float epsilon) const {
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (!M::NearlyEquals(m[row][col], mat2.m[row][col], epsilon)) {
                return false;
            }
        }
    }
    return true;
}

//* Matrices
Matrix2 Matrix2::operator+(const Matrix2& mat2) const {
    Matrix2 result = Zero;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            result.m[row][col] = m[row][col] + mat2.m[row][col];
        }
    }
    return result;
}

Matrix2 Matrix2::operator-(const Matrix2& mat2) const {
    Matrix2 result = Zero;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            result.m[row][col] = m[row][col] - mat2.m[row][col];
        }
    }
    return result;
}

Matrix2 Matrix2::operator*(const Matrix2& mat2) const {
    Matrix2 result(0);

    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            for (int k = 0; k < 2; k++) {
                result.m[row][col] += m[row][k] * mat2.m[k][col];
            }
        }
    }

    return result;
}

Matrix2& Matrix2::operator+=(const Matrix2& mat2) {
    return *this = *this + mat2;
}

Matrix2& Matrix2::operator-=(const Matrix2& mat2) {
    return *this = *this - mat2;
}

Matrix2& Matrix2::operator*=(const Matrix2& mat2) {
    return *this = *this * mat2;
}

//* Vectors
Vector2 Matrix2::operator*(const Vector2& vec2) const {
    return { m[0][0] * vec2.x + m[0][1] * vec2.y, m[1][0] * vec2.x + m[1][1] * vec2.y };
}

//* Scalars
Matrix2 Matrix2::operator*(const float scalar) const {
    Matrix2 result = Zero;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            result.m[row][col] = m[row][col] * scalar;
        }
    }
    return result;
}

Matrix2 Matrix2::operator/(const float scalar) const {
    Matrix2 result = Zero;
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            result.m[row][col] = m[row][col] / scalar;
        }
    }
    return result;
}

Matrix2& Matrix2::operator*=(const float scalar) {
    return *this = *this * scalar;
}

Matrix2& Matrix2::operator/=(const float scalar) {
    return *this = *this / scalar;
}

Matrix2 Matrix2::operator-() const {
    return *this * -1;
}

//* Equality
bool Matrix2::operator==(const Matrix2& mat2) const {
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (m[row][col] != mat2.m[row][col])
                return false;
        }
    }
    return true;
}

bool Matrix2::operator!=(const Matrix2& mat2) const {
    return !(*this == mat2);
}

//? Statics
Matrix2 const Matrix2::Zero = Matrix2(0);

Matrix2 const Matrix2::Identity = Matrix2(1, 0, 0, 1);

Matrix2 operator*(const float scalar, const Matrix2& mat2) {
    return mat2 * scalar;
}

//* Others
std::ostream& operator<<(std::ostream& os, const Matrix2& mat2) {
    os << "[ " << mat2.m[0][0] << "  " << mat2.m[0][1] << " ]\n";
    os << "[ " << mat2.m[1][0] << "  " << mat2.m[1][1] << " ]";
    return os;
}
} // namespace N::M
