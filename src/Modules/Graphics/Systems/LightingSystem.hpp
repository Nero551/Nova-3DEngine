#pragma once
#include "Core/OuterCore/ECS/System.hpp"
#include "Modules/Graphics/Resources/Uniformbuffer/Uniformbuffer.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N
{
struct LightingSystem : C::System
{
    U::CheckedPtr<Uniformbuffer> LightingBuffer;

    void Start() override;
    void Render() override;
};
} // namespace N
