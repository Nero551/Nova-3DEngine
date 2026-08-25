#include "LightingSystem.hpp"

#include "../../../World/Components/Transform3DComponent.hpp"
#include "../Components/LightComponent.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Uniforms/FloatUniform.hpp"
#include "../Uniforms/IntUniform.hpp"
#include "../Uniforms/Vector3Uniform.hpp"
#include "World/Events/EntityCreated.hpp"
#include "World/Events/EntityDestroyed.hpp"

namespace N {
static std::vector<unsigned int> Lights;

static void OnEntityCreated(const EntityCreated& event) {
    if (event.entity.HasComponent<LightComponent>()) {
        Lights.emplace_back(event.entity.Id);
    }
}

static void OnEntityDestroyed(const EntityDestroyed& event) {
    for (int i = 0; i < static_cast<int>(Lights.size()); i++) {
        if (event.entity.Id == Lights[i]) {
            Lights.erase(Lights.begin() + i);
        }
    }
}

void LightingSystem::Start() {
    Service::Get<EventBus>().Sub<EntityCreated>(OnEntityCreated);
    Service::Get<EventBus>().Sub<EntityDestroyed>(OnEntityDestroyed);
}

void LightingSystem::Render() {
    for (auto& entity : World::Get().Root->GetDescendants()) {
        if (!entity->HasComponent<Transform3DComponent, MaterialComponent>() || entity->HasComponent<LightComponent>()) {
            continue;
        }

        auto& materialComponent = entity->GetComponent<MaterialComponent>();

        materialComponent.Material->Shader->SetUniform(IntUniform("MAX_LIGHTS", World::Get().MaxLights));

        for (int i = 0; i < static_cast<int>(Lights.size()); i++) {
            auto& light = World::Get().FindEntity(Lights[i]);
            auto& lightComponent = light.GetComponent<LightComponent>();

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Color", i), lightComponent.Color));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Position", i), light.GetComponent<Transform3DComponent>().Position));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Ambient", i), lightComponent.Ambient));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Diffuse", i), lightComponent.Diffuse));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Specular", i), lightComponent.Specular));

            materialComponent.Material->Shader->SetUniform(
                IntUniform(std::format("LIGHTS[{}].Type", i), static_cast<int>(lightComponent.Type)));

            materialComponent.Material->Shader->SetUniform(
                Vector3Uniform(std::format("LIGHTS[{}].Direction", i), light.GetComponent<Transform3DComponent>().GetForward()));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].Constant", i), lightComponent.Constant));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].Linear", i), lightComponent.Linear));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].Quadratic", i), lightComponent.Quadratic));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].Intensity", i), lightComponent.Intensity));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].InnerCutOff", i), std::cos(lightComponent.InnerCutOff)));

            materialComponent.Material->Shader->SetUniform(
                FloatUniform(std::format("LIGHTS[{}].OuterCutOff", i), std::cos(lightComponent.OuterCutOff)));
        }
    }
}
} // namespace N
