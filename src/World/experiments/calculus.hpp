#pragma once

#include "Core/OuterCore/ECS/System.hpp"

namespace E {
struct calculus : System {
    void Start() override;

    void Update(double dt) override;

private:
    void TwoDimensionalProjection(float increase);
    void ThreeDimensionalProjection(float increase);
    void FourDimensionalProjection(float increase);
};
} // namespace E
