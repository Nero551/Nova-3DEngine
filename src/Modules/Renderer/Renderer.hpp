#pragma once

#include "Core/InnerCore/Module.hpp"
#include "Framebuffer.hpp"

namespace N {
struct Renderer : Module {
    Framebuffer Framebuffer;

protected:
    void OnStart() override;
    void OnBeginFrame(double dt) override;
    void OnRender() override;
    void AddSystems() override;
};
} // namespace N
