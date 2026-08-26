#include "Renderer.hpp"

#include "../../World/Components/Transform3DComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Primitives/Primitives.hpp"
#include "Resources/Shader/Uniforms/FloatUniform.hpp"
#include "Resources/Shader/Uniforms/Matrix3Uniform.hpp"
#include "Resources/Shader/Uniforms/Matrix4Uniform.hpp"
#include "Resources/Shader/Uniforms/Vector3Uniform.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/LightingSystem.hpp"

namespace N {
enum class FrameBufferTarget { ReadDraw = GL_FRAMEBUFFER, Read = GL_READ_FRAMEBUFFER, Draw = GL_DRAW_FRAMEBUFFER };

enum class TextureAttachment {
    Color = GL_COLOR_ATTACHMENT0,
    Depth = GL_DEPTH_ATTACHMENT,
    Stencil = GL_STENCIL_ATTACHMENT,
    DepthStencil = GL_DEPTH_STENCIL
};

struct FrameBuffer {
    unsigned int Id;
    FrameBufferTarget Target = FrameBufferTarget::ReadDraw;

    FrameBuffer() {
        glGenFramebuffers(1, &Id);
    }

    void Bind() {
        glBindFramebuffer(static_cast<GLenum>(Target), Id);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
            U::Logger::Info("FrameBuffer Usable, 🔥");
        }
    }

    void AttachTexture(TextureAttachment textureAttachment, const Texture& texture) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(textureAttachment), GL_TEXTURE_2D, texture.GetId(), 0);
    }
};

void Renderer::OnStart() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    // unsigned int fbo;
    // glGenFramebuffers(1, &fbo);
    //
    // U::Image image = {Engine::Get().Window.GetWidth(), Engine::Get().Window.GetWidth(), U::Image::ColorChannels::RGB, {}};
    // auto& texture = Service::Get<ResourceManager>().Load<Texture>("framebuffer", image);
    // texture.MinFilter = TextureFilter::Linear;
    // texture.MagFilter = TextureFilter::Linear;
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetId(), 0);
    //
    // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
    //     U::Logger::Info("FrameBuffer Usable, 🔥");
    // }
}

void Renderer::OnBeginFrame(double dt) {
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
}

void Renderer::AddSystems() {
    AddSystem<CameraSystem>();
    AddSystem<LightingSystem>();
}
} // namespace N
