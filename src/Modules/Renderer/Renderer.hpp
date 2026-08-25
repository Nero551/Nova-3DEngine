#pragma once

#include "Core/InnerCore/Module.hpp"

namespace N {
struct Renderer : Module {
protected:
    void OnStart() override;
    void OnBeginFrame(double dt) override;
    void OnRender() override;

private:
    void AddSystems() override;
};
} // namespace N
