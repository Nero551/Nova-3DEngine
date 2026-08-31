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
void LightingSystem::Start() {
    auto& resources = Service::Get<ResourceManager>();

    LightingBuffer = &resources.Load<Uniformbuffer>("[Renderer] Lighting Buffer");
    LightingBuffer->Binding = 1;

    constexpr size_t LightStride = 144;
    constexpr size_t HeaderSize = 16;

    LightingBuffer->Size = HeaderSize + World::Get().MaxLights * LightStride;
}

void LightingSystem::Render() {
    auto& world = World::Get();

    LightingBuffer->Set(static_cast<int>(world.Query.With<LightComponent, Transform3DComponent>().EntityIds.size()), 0);

    int i = 0;
    for (auto [entityId, lightComponent, transform] : world.Query.With<LightComponent, Transform3DComponent>()) {
        constexpr size_t LightStride = 144;
        const size_t LightOffset = 16 + i * LightStride;

        LightingBuffer->Set(static_cast<int>(lightComponent.Type), LightOffset + 0);
        LightingBuffer->Set(transform.GetForward(), LightOffset + 16, 4);
        LightingBuffer->Set(lightComponent.Color, LightOffset + 32, 4);
        LightingBuffer->Set(transform.Position, LightOffset + 48, 4);
        LightingBuffer->Set(lightComponent.Ambient, LightOffset + 64, 4);
        LightingBuffer->Set(lightComponent.Diffuse, LightOffset + 80, 4);
        LightingBuffer->Set(lightComponent.Specular, LightOffset + 96, 4);

        LightingBuffer->Set(lightComponent.Intensity, LightOffset + 112);
        LightingBuffer->Set(lightComponent.Constant, LightOffset + 116);
        LightingBuffer->Set(lightComponent.Linear, LightOffset + 120);
        LightingBuffer->Set(lightComponent.Quadratic, LightOffset + 124);
        LightingBuffer->Set(lightComponent.InnerCutOff, LightOffset + 128);
        LightingBuffer->Set(lightComponent.OuterCutOff, LightOffset + 132);

        ++i;
    }

    LightingBuffer->Bind();
}
} // namespace N
