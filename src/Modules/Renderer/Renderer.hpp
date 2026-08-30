#pragma once

#include "Core/InnerCore/Module.hpp"
#include "RenderBatch.hpp"
#include "Resources/Framebuffer/Framebuffer.hpp"
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"
#include "Resources/Uniformbuffer/Uniformbuffer.hpp"

namespace N {
struct Renderer : Module {
    U::CheckedPtr<Framebuffer> Framebuffer{ "Renderer has no Framebuffer to render on" };
    U::CheckedPtr<Material> ScreenMaterial{ "Renderer has no Screen Material to render on" };
    U::CheckedPtr<Mesh> ScreenMesh{ "Renderer has no Screen Mesh to render on" };
    U::CheckedPtr<Uniformbuffer> GUniformbuffer{ "Renderer has no Uniform buffer to use" };
    std::unordered_map<std::string, RenderBatch> Batches;

protected:
    void AddSystems() override;

    void SetupFramebuffers();

    void OnStart() override;
    void OnBeginFrame(double dt) override;

    void RenderWorld();
    void PresentFramebuffer();

    void OnRender() override;

    void OnStop() override;
};
} // namespace N
