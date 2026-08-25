#include "Renderer.hpp"

#include "../../World/Components/Transform3DComponent.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/OuterCore/ECS/Entity.hpp"
#include "Primitives/Primitives.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/LightingSystem.hpp"
#include "Uniforms/FloatUniform.hpp"
#include "Uniforms/Matrix3Uniform.hpp"
#include "Uniforms/Matrix4Uniform.hpp"
#include "Uniforms/Vector3Uniform.hpp"

namespace N {
void Renderer::OnStart() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Renderer::OnBeginFrame(double dt) {
    glClearColor(0.05, 0.025, 0.05, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

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

            materialComponent.Material->Use();

            materialComponent.Material->Shader->SetUniform(FloatUniform("TIME", static_cast<float>(Engine::Get().GetTime())));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform("VIEW_POSITION", camera->GetComponent<Transform3DComponent>().Position));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("MODEL_MATRIX", transformComponent.GetModelMatrix()));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("VIEW_MATRIX", view));

            materialComponent.Material->Shader->SetUniform(Matrix4Uniform("PROJECTION_MATRIX", projection));

            materialComponent.Material->Shader->SetUniform(Matrix3Uniform("NORMAL_MATRIX", transformComponent.GetNormalMatrix()));
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
