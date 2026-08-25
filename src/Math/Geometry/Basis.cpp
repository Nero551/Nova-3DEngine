#include "Basis.hpp"

namespace N::M {
Matrix4 Basis::GetMatrix() const {
    Matrix4 basisMatrix = Matrix4::Identity;

    basisMatrix.m[0][0] = Right.x;
    basisMatrix.m[1][0] = Right.y;
    basisMatrix.m[2][0] = Right.z;

    basisMatrix.m[0][1] = Up.x;
    basisMatrix.m[1][1] = Up.y;
    basisMatrix.m[2][1] = Up.z;

    basisMatrix.m[0][2] = Forward.x;
    basisMatrix.m[1][2] = Forward.y;
    basisMatrix.m[2][2] = Forward.z;

    return basisMatrix;
}

Matrix4 Basis::GetInverseMatrix() const {
    return GetMatrix().Transpose();
}
} // namespace N::M
