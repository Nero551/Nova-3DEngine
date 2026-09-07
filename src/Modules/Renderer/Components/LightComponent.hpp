#pragma once

#include "../Enums/LightType.hpp"
#include "Core/OuterCore/ECS/Component.hpp"
#include "Math/Common/Trigonometry.hpp"
#include "Math/Vector/Vector3.hpp"

namespace N {
struct LightComponent : Component {
    LightType Type = LightType::Directional;
    M::Vector3 Color = M::Vector3{ 1 };
    M::Vector3 Ambient = M::Vector3{ 0.8 };
    M::Vector3 Diffuse = M::Vector3{ 1 };
    M::Vector3 Specular = M::Vector3{ 1 };

    float Intensity = 1;
    float Constant = 1.0f;
    float Linear = 0.009f;
    float Quadratic = 0.032f;
    float InnerCutOff = M::Rad(7.5);
    float OuterCutOff = M::Rad(17.5);
};
} // namespace N
