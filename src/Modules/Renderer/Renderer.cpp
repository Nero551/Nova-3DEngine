#include "Renderer.hpp"

#include "../../World/Components/Transform3DComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Framebuffer.hpp"
#include "Primitives/Primitives.hpp"
#include "Resources/Shader/Uniforms/FloatUniform.hpp"
#include "Resources/Shader/Uniforms/Matrix3Uniform.hpp"
#include "Resources/Shader/Uniforms/Matrix4Uniform.hpp"
#include "Resources/Shader/Uniforms/Vector3Uniform.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/LightingSystem.hpp"

namespace N {
void Renderer::OnStart() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    auto& resources = Service::Get<ResourceManager>();
    auto& window = Engine::Get().Window;

    auto& colorTexture = resources.Load<Texture>("color-buffer");
    colorTexture.Target = TextureTarget::Texture2D;
    colorTexture.InternalFormat = TextureInternalFormat::RGB8;
    colorTexture.Format = TextureFormat::RGB;
    colorTexture.DataType = TextureDataType::UnsignedByte;
    colorTexture.Width = window.GetWidth();
    colorTexture.Height = window.GetHeight();
    colorTexture.AutoMipmaps = false;
    colorTexture.MagFilter = TextureFilter::Linear;
    colorTexture.MinFilter = TextureFilter::Linear;
    colorTexture.Load();

    Renderbuffer depthstencilBuffer;
    depthstencilBuffer.Height = window.GetHeight();
    depthstencilBuffer.Width = window.GetWidth();
    depthstencilBuffer.InternalFormat = TextureInternalFormat::Depth24Stencil8;

    Framebuffer.Bind();
    Framebuffer.AttachTexture(TextureAttachment::Color, colorTexture);
    Framebuffer.AttachRenderBuffer(TextureAttachment::DepthStencil, depthstencilBuffer);
    Framebuffer.IsComplete();

    glfwSetFramebufferSizeCallback(Engine::Get().Window.GetGlfwWindow(), [](GLFWwindow*, const int w, const int h) {
        glViewport(0, 0, w, h);
        auto& renderer = Engine::Get().GetModule<Renderer>();
        renderer.Framebuffer.Resize(w, h);
    });
}

void Renderer::OnBeginFrame(double dt) {
    Framebuffer.Bind();

    glClearColor(0.08, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


// TODO- if there is multiple semi-transparent objects behind each other , depth testing breaks blending.
//  fix this by classifying render passes by transparency, pairs well with future render batches / instancing.
//  for ordering semi-transparent object by distance , use a map , it auto sorts.
void Renderer::OnRender() {
    auto& camera = World::Get().ActiveCamera;

    M::Matrix4 projection = camera->GetComponent<CameraComponent>().GetProjectionMatrix();
    M::Matrix4 view = GetSystem<CameraSystem>().GetViewMatrix();

    for (auto& entity : World::Get().Root->GetDescendants()) {
        if (!entity->HasComponent<Transform3DComponent>()) {
            continue;
        }
        auto& transformComponent = entity->GetComponent<Transform3DComponent>();

        if (entity->HasComponent<MaterialComponent>()) {
            auto& materialComponent = entity->GetComponent<MaterialComponent>();

            if (materialComponent.Material->Shader->HotReload == true) {
                materialComponent.Material->Shader->Reload();
            }

            materialComponent.Material->Shader->SetUniform(FloatUniform("TIME", static_cast<float>(Engine::Get().GetTime())));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform("VIEW_POSITION", camera->GetComponent<Transform3DComponent>().GlobalPosition));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("MODEL_MATRIX", transformComponent.GetModelMatrix()));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("VIEW_MATRIX", view));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("PROJECTION_MATRIX", projection));

            materialComponent.Material->Shader->SetUniform(Matrix3Uniform("NORMAL_MATRIX", transformComponent.GetNormalMatrix()));

            materialComponent.Material->Use();
        }

        if (entity->HasComponent<MeshComponent>()) {
            auto& meshComponent = entity->GetComponent<MeshComponent>();
            meshComponent.Mesh->Draw();
        }
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, Framebuffer.GetId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Renderer::AddSystems() {
    AddSystem<CameraSystem>();
    AddSystem<LightingSystem>();
}
} // namespace N
