#pragma once

#include "Core/InnerCore/Module.hpp"
#include "Framebuffer.hpp"
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"

namespace N {
// TODO- make frame buffers & render buffers a resource , make all stuff here pointers , all owned by resource manager
struct Renderer : Module {
    U::CheckedPtr<Framebuffer> Framebuffer;
    U::CheckedPtr<Material> ScreenMaterial;
    U::CheckedPtr<Mesh> ScreenMesh;
    // Shader ScreenShader;
    // ShaderSource ScreenVertex;
    // ShaderSource ScreenFragment;

protected:
    void OnStart() override;
    void OnBeginFrame(double dt) override;
    void OnRender() override;
    void AddSystems() override;
};
} // namespace N
