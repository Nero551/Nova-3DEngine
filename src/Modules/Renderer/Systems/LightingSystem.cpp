#include "LightingSystem.hpp"

#include "../../../World/Components/Transform3DComponent.hpp"
#include "../Components/LightComponent.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Resources/Shader/Uniforms/FloatUniform.hpp"
#include "../Resources/Shader/Uniforms/IntUniform.hpp"
#include "../Resources/Shader/Uniforms/Vector3Uniform.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Modules/Input/Enums/Keys.hpp"
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

    auto& resources = Service::Get<ResourceManager>();

    LightingBuffer = &resources.Load<Uniformbuffer>("[Lighting System] Lighting Buffer");
    LightingBuffer->Binding = 1;

    constexpr size_t LightStride = 144;
    constexpr size_t HeaderSize = 16;

    LightingBuffer->Size = HeaderSize + World::Get().MaxLights * LightStride;
}

void LightingSystem::Render() {
    LightingBuffer->Set(static_cast<int>(Lights.size()), 0);

    for (int i = 0; i < Lights.size(); i++) {
        constexpr size_t LightStride = 144;
        const size_t LightOffset = 16 + i * LightStride;

        auto& light = World::Get().FindEntity(Lights[i]);
        auto& lightComponent = light.GetComponent<LightComponent>();

        LightingBuffer->Set(static_cast<int>(lightComponent.Type), LightOffset + 0);
        LightingBuffer->Set(light.GetComponent<Transform3DComponent>().GetForward(), LightOffset + 16, 4);
        LightingBuffer->Set(lightComponent.Color, LightOffset + 32, 4);
        LightingBuffer->Set(light.GetComponent<Transform3DComponent>().Position, LightOffset + 48, 4);
        LightingBuffer->Set(lightComponent.Ambient, LightOffset + 64, 4);
        LightingBuffer->Set(lightComponent.Diffuse, LightOffset + 80, 4);
        LightingBuffer->Set(lightComponent.Specular, LightOffset + 96, 4);

        LightingBuffer->Set(lightComponent.Intensity, LightOffset + 112);
        LightingBuffer->Set(lightComponent.Constant, LightOffset + 116);
        LightingBuffer->Set(lightComponent.Linear, LightOffset + 120);
        LightingBuffer->Set(lightComponent.Quadratic, LightOffset + 124);
        LightingBuffer->Set(lightComponent.InnerCutOff, LightOffset + 128);
        LightingBuffer->Set(lightComponent.OuterCutOff, LightOffset + 132);
    }

    LightingBuffer->Bind();
}
} // namespace N
