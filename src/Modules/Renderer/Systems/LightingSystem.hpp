#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Modules/Renderer/Resources/Uniformbuffer/Uniformbuffer.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
struct LightingSystem : System {
    void Start() override;
    void Render() override;
};
} // namespace N
