#pragma once

#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Module.hpp"
#include "RenderBatch.hpp"
#include "Resources/Framebuffer/Framebuffer.hpp"
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"
#include "Resources/Uniformbuffer/Uniformbuffer.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
struct Renderer : Module {
    U::CheckedPtr<Framebuffer> Framebuffer{ "Renderer has no Framebuffer to use" };
    U::CheckedPtr<struct Framebuffer> MSAAFramebuffer{ "Renderer has no MSAA Framebuffer to use" };
    int MSAASamples = 8;

    U::CheckedPtr<Material> ScreenMaterial{ "Renderer has no Screen Material to render on" };
    U::CheckedPtr<Mesh> ScreenMesh{ "Renderer has no Screen Mesh to render on" };
    U::CheckedPtr<Uniformbuffer> GUniformbuffer{ "Renderer has no Uniform buffer to use" };

    std::unordered_map<std::string, RenderBatch> Batches;

protected:
    void SetupFramebuffer();
    void SetupMSAAFrameBuffer();
    void PresentFramebuffer();

    void Start() override;
    void BeginFrame(double dt) override;

    void RenderWorld();
    void FillBatches(
        Transform3DComponent& transformComponent, MaterialComponent& materialComponent, MeshComponent& meshComponent);

    void Render() override;
    void Update(double dt) override;
    void FixedUpdate(double fdt) override;

    void Stop() override;
};
} // namespace N
