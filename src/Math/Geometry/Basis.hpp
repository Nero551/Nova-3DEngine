#pragma once
#include "../Vector/Vector3.hpp"
#include "Math/Matrix/Matrix4.hpp"

namespace N::M
{
struct Basis
{
    Vector<3> Right = Vector<3>::Right();
    Vector<3> Up = Vector<3>::Up();
    Vector<3> Forward = Vector<3>::Forward();

    [[nodiscard]] Matrix<4, 4> GetInverseMatrix() const;
    [[nodiscard]] Matrix<4, 4> GetMatrix() const;
};
} // namespace N::M
