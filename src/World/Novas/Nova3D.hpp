#pragma once
#include "Nova.hpp"
#include "World/Components/Transform3DComponent.hpp"

namespace N {
struct Nova3D : Nova {
    void Initialize() override {
        Nova::Initialize();
        World::Get().Query.Pool<Transform3DComponent>().Add(Id);
    }
};
} // namespace N
