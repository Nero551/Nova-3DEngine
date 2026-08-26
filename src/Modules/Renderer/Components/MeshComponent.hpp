#pragma once
#include "Core/OuterCore/ECS/Component.hpp"
#include "Modules/Renderer/Mesh/Mesh.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
struct MeshComponent : Component {
    U::CheckedPtr<Mesh> Mesh{ "Mesh Component Has No Mesh Assigned" };
};
} // namespace N
