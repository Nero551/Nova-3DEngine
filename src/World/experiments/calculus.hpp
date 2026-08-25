#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace N {
struct calculus : System {
    void Start() override;

    void FixedUpdate(double dt) override;

private:
    void TwoDimensionalProjection(float increase);
    void ThreeDimensionalProjection(float increase);
    void FourDimensionalProjection(float increase);
};
} // namespace N
