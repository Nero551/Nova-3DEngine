#include "Renderer.hpp"

#include "../../World/Components/Transform3DComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Primitives/Primitives.hpp"
#include "Resources/Framebuffer/Framebuffer.hpp"
#include "Resources/Shader/Uniforms/FloatUniform.hpp"
#include "Resources/Shader/Uniforms/IntUniform.hpp"
#include "Resources/Shader/Uniforms/Matrix3Uniform.hpp"
#include "Resources/Shader/Uniforms/Matrix4Uniform.hpp"
#include "Resources/Shader/Uniforms/Vector3Uniform.hpp"
#include "Resources/Texture/Texture2D.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/LightingSystem.hpp"

namespace N {
void Renderer::AddSystems() {
    AddSystem<CameraSystem>();
    AddSystem<LightingSystem>();
}

void Renderer::SetupFramebuffers() {
    std::vector vertices = { Vertex({ -1.0f, -1.0f, 0.0f, 1.0f }, {}, { 0.0f, 0.0f }, {}),
        Vertex({ 1.0f, -1.0f, 0.0f, 1.0f }, {}, { 1.0f, 0.0f }, {}),
        Vertex({ 1.0f, 1.0f, 0.0f, 1.0f }, {}, { 1.0f, 1.0f }, {}),
        Vertex({ -1.0f, 1.0f, 0.0f, 1.0f }, {}, { 0.0f, 1.0f }, {}) };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };


    auto& resources = Service::Get<ResourceManager>();
    Framebuffer = &resources.Load<struct Framebuffer>("[Renderer] Framebuffer");
    Framebuffer->Target = FrameBufferTarget::ReadDraw;

    ScreenMesh = &resources.Load<Mesh>("[Renderer] Screen Mesh");
    ScreenMaterial = &resources.Load<Material>("[Renderer] Screen Material");
    auto& screenShader = resources.Load<Shader>("[Renderer] Screen Shader");
    auto& screenVert =
        resources.Load<ShaderSource>("[Renderer] Screen Vertex", "Assets/Shaders/screen.vert", ShaderStage::Vertex);
    auto& screenFrag =
        resources.Load<ShaderSource>("[Renderer] Screen Fragment", "Assets/Shaders/screen.frag", ShaderStage::Fragment);

    screenShader.AssignSource(screenVert);
    screenShader.AssignSource(screenFrag);

    ScreenMaterial->Shader = &screenShader;
    ScreenMaterial->Depth.Enabled = false;
    ScreenMaterial->Stencil.Enabled = false;
    ScreenMaterial->Blend.Enabled = false;

    ScreenMesh->Vertices = vertices;
    ScreenMesh->Indices = indices;
    ScreenMesh->CullMode = CullMode::None;
    // ScreenMesh->RenderMode = RenderMode::Wireframe;

    auto& window = Engine::Get().Window;

    auto& colorTexture = resources.Load<Texture2D>("colorbuffer");
    colorTexture.InternalFormat = TextureInternalFormat::RGB8;
    colorTexture.Format = TextureFormat::RGB;
    colorTexture.DataType = DataType::UnsignedByte;
    colorTexture.Width = window.GetWidth();
    colorTexture.Height = window.GetHeight();
    colorTexture.AutoMipmaps = false;
    colorTexture.MagFilter = TextureFilter::Linear;
    colorTexture.MinFilter = TextureFilter::Linear;

    auto& depthstencilBuffer = resources.Load<Renderbuffer>("[Renderer] DepthStencil Render Buffer");
    depthstencilBuffer.Height = window.GetHeight();
    depthstencilBuffer.Width = window.GetWidth();
    depthstencilBuffer.InternalFormat = TextureInternalFormat::Depth24Stencil8;

    Framebuffer->Bind();
    Framebuffer->AttachTexture(FramebufferAttachment::Color0, colorTexture);
    Framebuffer->AttachRenderBuffer(FramebufferAttachment::DepthStencil, depthstencilBuffer);
    Framebuffer->IsComplete();
}

void Renderer::OnStart() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glEnable(GL_PROGRAM_POINT_SIZE);

    auto& resources = Service::Get<ResourceManager>();
    GUniformbuffer = &resources.Load<Uniformbuffer>("[Renderer] Global Uniform buffer");
    GUniformbuffer->Size = 160;

    SetupFramebuffers();
    glfwSetFramebufferSizeCallback(Engine::Get().Window.GetGlfwWindow(), [](GLFWwindow*, const int w, const int h) {
        glViewport(0, 0, w, h);
        auto& renderer = Engine::Get().GetModule<Renderer>();
        renderer.Framebuffer->Resize(w, h);
    });
}

void Renderer::OnBeginFrame(double dt) {
    Framebuffer->Bind();

    glClearColor(0.08, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


struct RenderBatch {
    U::CheckedPtr<Material> Material;
    U::CheckedPtr<Mesh> Mesh;
    std::vector<M::Matrix4> ModelMatrices;
    std::vector<M::Matrix3> NormalMatrices;
};

void Renderer::RenderWorld() {
    auto& camera = World::Get().ActiveCamera;

    M::Matrix4 projection = camera->GetComponent<CameraComponent>().GetProjectionMatrix();
    M::Matrix4 view = GetSystem<CameraSystem>().GetViewMatrix();

    std::vector<RenderBatch> Batches;

    GUniformbuffer->Set(view.Transpose(), 0);
    GUniformbuffer->Set(projection.Transpose(), 64);
    GUniformbuffer->Set(Engine::Get().GetTime(), 128);
    GUniformbuffer->Set(camera->GetComponent<Transform3DComponent>().GlobalPosition, 144);
    GUniformbuffer->Bind();


    for (auto& entity : World::Get().Root->GetDescendants()) {
        if (entity->HasComponent<MaterialComponent, MeshComponent, Transform3DComponent>()) {
            auto& transformComponent = entity->GetComponent<Transform3DComponent>();
            auto& materialComponent = entity->GetComponent<MaterialComponent>();
            auto& meshComponent = entity->GetComponent<MeshComponent>();

            auto it = std::ranges::find_if(Batches, [meshComponent, materialComponent](const RenderBatch& batch) mutable {
                return batch.Mesh == meshComponent.Mesh && batch.Material == materialComponent.Material;
            });

            if (it == Batches.end()) {
                RenderBatch batch;
                batch.Mesh = meshComponent.Mesh;
                batch.Material = materialComponent.Material;
                batch.ModelMatrices.push_back(transformComponent.GetModelMatrix());
                batch.NormalMatrices.push_back(transformComponent.GetNormalMatrix());

                Batches.push_back(batch);
            }
            else {
                it->ModelMatrices.push_back(transformComponent.GetModelMatrix().Transpose());
                it->NormalMatrices.push_back(transformComponent.GetNormalMatrix().Transpose());
            }

            if (materialComponent.Material->Shader->HotReload == true) {
                materialComponent.Material->Shader->Reload();
            }
        }
    }


    for (auto& batch : Batches) {
        batch.Material->Use();
        for (auto& matrix : batch.ModelMatrices) {
            std::cout << matrix << '\n';
        }

        int instanceCount = batch.ModelMatrices.size();
        ArrayBuffer instanceVBO;
        instanceVBO.Generate(batch.ModelMatrices);

        batch.Mesh->VAO.Bind();
        instanceVBO.Bind();
        batch.Mesh->VAO.SetMatrix4AttribPointer(4);
        batch.Mesh->VAO.SetMatrix4AttribDivisor(4, 1);

        batch.Mesh->DrawInstanced(instanceCount);
    }
}

void Renderer::PresentFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.08, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    int i = 0;
    for (auto& texture : Framebuffer->TextureAttachments | std::views::values) {
        ScreenMaterial->AssignTexture(*texture, i);
        i++;
    }

    ScreenMaterial->Shader->SetUniform(FloatUniform("TIME", static_cast<float>(Engine::Get().GetTime())));

    ScreenMaterial->Use();
    ScreenMesh->Draw();
}

// TODO- if there is multiple semi-transparent objects behind each other , depth testing breaks blending.
//  fix this by classifying render passes by transparency, pairs well with future render batches / instancing.
//  for ordering semi-transparent object by distance , use a map , it auto sorts.
void Renderer::OnRender() {
    RenderWorld();
    PresentFramebuffer();
}

void Renderer::OnStop() {
    const auto& texture = Framebuffer->TextureAttachments.at(FramebufferAttachment::Color0);

    std::vector<unsigned char> pixels(static_cast<size_t>(texture->Width) * static_cast<size_t>(texture->Height) * 3);

    glBindTexture(GL_TEXTURE_2D, texture->GetId());

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    U::Image image = {
        texture->Width,
        texture->Height,
        U::Image::ColorChannels::RGB,
        pixels,

    };
    image.SaveToDiskPNG("Assets/LastFrame.png", true);
}
} // namespace N
