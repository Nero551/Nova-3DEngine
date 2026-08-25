#pragma once
#include "Nova.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
struct Nova3D : Nova {
    void Initialize() override {
        Nova::Initialize();
        AddComponent<Transform3DComponent>();
    }
};
} // namespace N
