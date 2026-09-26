#pragma once
#include "../Matrix/Matrix4.hpp"
#include "../Vector/Vector3.hpp"

namespace N::M
{
struct Basis
{
    Vector<3> Right = Vector<3>::Right();
    Vector<3> Up = Vector<3>::Up();
    Vector<3> Forward = Vector<3>::Forward();

    [[nodiscard]] Matrix4 GetInverseMatrix() const;
    [[nodiscard]] Matrix4 GetMatrix() const;
};
} // namespace N::M
