#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace N
{
struct calculus : System
{
    void Start() override;

    void Update(double dt) override;

  private:
    void TwoDimensionalProjection(int increase);
    void ThreeDimensionalProjection(int increase);
    void FourDimensionalProjection(int increase);
};
} // namespace N
