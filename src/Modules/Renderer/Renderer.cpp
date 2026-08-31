#include "Renderer.hpp"

#include <tracy/Tracy.hpp>
#include "../../World/Components/Transform3DComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Primitives/Primitives.hpp"
#include "RenderBatch.hpp"
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
void Renderer::SetupFramebuffer() {
    const std::vector vertices = { Vertex({ -1.0f, -1.0f, 0.0f, 1.0f }, {}, { 0.0f, 0.0f }, {}),
        Vertex({ 1.0f, -1.0f, 0.0f, 1.0f }, {}, { 1.0f, 0.0f }, {}),
        Vertex({ 1.0f, 1.0f, 0.0f, 1.0f }, {}, { 1.0f, 1.0f }, {}),
        Vertex({ -1.0f, 1.0f, 0.0f, 1.0f }, {}, { 0.0f, 1.0f }, {}) };

    const std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
    auto& resources = Service::Get<ResourceManager>();
    auto& window = Engine::Get().Window;

    Framebuffer = &resources.Load<struct Framebuffer>("[Renderer] Framebuffer");

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

    auto& screenTexture = resources.Load<Texture2D>("COLOR_BUFFER");
    screenTexture.Width = window.GetWidth();
    screenTexture.Height = window.GetHeight();
    screenTexture.InternalFormat = TextureInternalFormat::RGB8;
    screenTexture.Format = TextureFormat::RGB;
    screenTexture.DataType = DataType::UnsignedByte;
    screenTexture.AutoMipmaps = false;
    screenTexture.MagFilter = TextureFilter::Linear;
    screenTexture.MinFilter = TextureFilter::Linear;

    Framebuffer->AttachTexture(FramebufferAttachment::Color0, screenTexture);

    glfwSetFramebufferSizeCallback(Engine::Get().Window.GetGlfwWindow(), [](GLFWwindow*, const int w, const int h) {
        glViewport(0, 0, w, h);
        auto& renderer = Engine::Get().GetModule<Renderer>();
        renderer.MSAAFramebuffer->Resize(w, h);
        renderer.Framebuffer->Resize(w, h);
    });
}

void Renderer::SetupMSAAFrameBuffer() {
    auto& resources = Service::Get<ResourceManager>();
    auto& window = Engine::Get().Window;

    MSAAFramebuffer = &resources.Load<struct Framebuffer>("[Renderer] MSAA Framebuffer");
    MSAAFramebuffer->Target = FrameBufferTarget::ReadDraw;

    auto& colorBuffer = resources.Load<Renderbuffer>("[Renderer] MSAA Color Render Buffer");
    colorBuffer.Width = window.GetWidth();
    colorBuffer.Height = window.GetHeight();
    colorBuffer.Samples = MSAASamples;
    colorBuffer.InternalFormat = TextureInternalFormat::RGB8;

    auto& depthstencilBuffer = resources.Load<Renderbuffer>("[Renderer] MSAA DepthStencil Render Buffer");
    depthstencilBuffer.Height = window.GetHeight();
    depthstencilBuffer.Width = window.GetWidth();
    depthstencilBuffer.Samples = MSAASamples;
    depthstencilBuffer.InternalFormat = TextureInternalFormat::Depth24Stencil8;

    MSAAFramebuffer->AttachRenderBuffer(FramebufferAttachment::Color0, colorBuffer);
    MSAAFramebuffer->AttachRenderBuffer(FramebufferAttachment::DepthStencil, depthstencilBuffer);
}

void Renderer::PresentFramebuffer() {
    auto& window = Engine::Get().Window;
    int width = window.GetWidth();
    int height = window.GetHeight();

    MSAAFramebuffer->Blit(*Framebuffer, width, height, width, height, BufferBit::Color);

    MSAAFramebuffer->Unbind();

    glClearColor(0.08, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    int i = 0;
    for (auto& texture : Framebuffer->TextureAttachments | std::views::values) {
        ScreenMaterial->AssignTexture(*texture, i);
        i++;
    }

    ScreenMaterial->Use();
    ScreenMesh->Draw();
}

void Renderer::Start() {
    AddSystem<CameraSystem>();
    AddSystem<LightingSystem>();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);

    auto& resources = Service::Get<ResourceManager>();
    GUniformbuffer = &resources.Load<Uniformbuffer>("[Renderer] Global Uniform buffer");
    GUniformbuffer->Size = 160;

    SetupMSAAFrameBuffer();
    SetupFramebuffer();

    GetSystem<LightingSystem>().Start();
}

void Renderer::BeginFrame(double dt) {
    MSAAFramebuffer->Bind();

    glClearColor(0.08, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::RenderWorld() {
    const auto& camera = World::Get().ActiveCamera;
    auto& query = World::Get().Query;

    const M::Matrix4 projection = query.Pool<CameraComponent>().GetComponentById(camera->Id).GetProjectionMatrix();
    const M::Matrix4 view = GetSystem<CameraSystem>().GetViewMatrix();

    GUniformbuffer->Set(view.Transpose(), 0);
    GUniformbuffer->Set(projection.Transpose(), 64);
    GUniformbuffer->Set(Engine::Get().GetTime(), 128);
    GUniformbuffer->Set(query.Pool<Transform3DComponent>().GetComponentById(camera->Id).GlobalPosition, 144);
    GUniformbuffer->Bind();

    for (auto& batch : Batches | std::views::values) {
        batch.Instances.clear();
    }

    for (auto [entityId, transformComponent, meshComponent, materialComponent] :
        query.With<Transform3DComponent, MeshComponent, MaterialComponent>()) {
        if (materialComponent.Material->Shader->HotReload == true) {
            materialComponent.Material->Shader->Reload();
        }

        FillBatches(transformComponent, materialComponent, meshComponent);
    }
    for (auto& batch : Batches | std::views::values) {
        batch.Render();
    }
}

void Renderer::FillBatches(
    Transform3DComponent& transformComponent, MaterialComponent& materialComponent, MeshComponent& meshComponent) {
    auto it = Batches.find(meshComponent.Mesh->Name + materialComponent.Material->Name);

    if (it == Batches.end()) {
        const std::string name = meshComponent.Mesh->Name + materialComponent.Material->Name;

        it = Batches.try_emplace(name, meshComponent.Mesh, materialComponent.Material).first;
    }
    it->second.Instances.emplace_back(
        transformComponent.GetModelMatrix().Transpose(), transformComponent.GetNormalMatrix().Transpose());
}

// TODO- if there is multiple semi-transparent objects behind each other , depth testing breaks blending.
//  fix this by classifying render passes by transparency, pairs well with future render batches / instancing.
//  for ordering semi-transparent object by distance , use a map , it auto sorts.
void Renderer::Render() {
    GetSystem<LightingSystem>().Render();
    RenderWorld();
    PresentFramebuffer();
}

void Renderer::Update(double dt) {
    GetSystem<CameraSystem>().Update(dt);
}

void Renderer::FixedUpdate(double fdt) {
}

void Renderer::Stop() {
    const auto& texture = Framebuffer->TextureAttachments.at(FramebufferAttachment::Color0);

    std::vector<unsigned char> pixels(static_cast<size_t>(texture->Width) * static_cast<size_t>(texture->Height) * 3);

    glBindTexture(GL_TEXTURE_2D, texture->GetId());

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    U::Image image = { texture->Width, texture->Height, U::Image::ColorChannels::RGB, pixels };
    image.SaveToDiskPNG("Assets/LastFrame.png", true);
}
} // namespace N
