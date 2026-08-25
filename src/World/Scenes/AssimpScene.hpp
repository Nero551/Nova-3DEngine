#pragma once

#include "Core/OuterCore/Scene.hpp"

namespace N {
struct AssimpScene : Scene {
    AssimpScene(const std::string& filepath);
};
} // namespace N
