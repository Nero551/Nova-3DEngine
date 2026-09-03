#pragma once

#include <deque>

#include "Core/InnerCore/Module.hpp"

namespace N {
struct Profiling : Module {
    double FrameMs;
    int FrameCount;

protected:
    void Start() override;

    void Update(double dt) override;

private:
    std::deque<double> FrameTimes;
};
} // namespace N
