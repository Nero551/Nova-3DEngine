#pragma once

#include <deque>

#include "Core/InnerCore/Module.hpp"

namespace N {
struct Profiling : Module {
    double FrameMs;
    int FrameCount;

protected:
    void OnStart() override;

    void OnUpdate(double dt) override;

private:
    std::deque<double> FrameTimes;

    void AddSystems() override;
};
} // namespace N
