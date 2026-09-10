#include "Basis.hpp"

namespace N::M
{
Matrix4 Basis::GetInverseMatrix() const
{
    return GetMatrix().Transpose();
}

Matrix4 Basis::GetMatrix() const
{
    Matrix4 basisMatrix = Matrix4::Identity;

    basisMatrix(0, 0) = Right.x;
    basisMatrix(1, 0) = Right.y;
    basisMatrix(2, 0) = Right.z;

    basisMatrix(0, 1) = Up.x;
    basisMatrix(1, 1) = Up.y;
    basisMatrix(2, 1) = Up.z;

    basisMatrix(0, 2) = Forward.x;
    basisMatrix(1, 2) = Forward.y;
    basisMatrix(2, 2) = Forward.z;

    return basisMatrix;
}
} // namespace N::M