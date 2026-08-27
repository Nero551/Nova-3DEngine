#pragma once
#include "Texture.hpp"

namespace N {
struct Cubemap : Texture {
    U::Image Front;
    U::Image Back;
    U::Image Top;
    U::Image Bottom;
    U::Image Right;
    U::Image Left;
    Cubemap(const std::string& name) : Texture(name, TextureTarget::CubeMap) {
    }

    void Generate() override;
};
} // namespace N
