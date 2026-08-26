#pragma once
#include "Math/Vector/Vector2.hpp"
#include "Math/Vector/Vector3.hpp"
#include "Math/Vector/Vector4.hpp"

namespace N {
struct Vertex {
    M::Vector4 Position;
    M::Vector4 Color;
    M::Vector2 UV;
    M::Vector3 Normal;

    Vertex(const M::Vector4 pos, const M::Vector4 col, const M::Vector2 uv, const M::Vector3 normal) :
        Position(pos), Color(col), UV(uv), Normal(normal) {
    }
};
} // namespace N
