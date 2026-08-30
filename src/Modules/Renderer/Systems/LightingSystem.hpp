#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Modules/Renderer/Resources/Uniformbuffer/Uniformbuffer.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
struct LightingSystem : System {
    U::CheckedPtr<Uniformbuffer> LightingBuffer;

    void Start() override;
    void Render() override;
};
} // namespace N
