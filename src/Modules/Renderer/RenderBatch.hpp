#pragma once
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"

namespace N {
struct RenderBatch {
    U::CheckedPtr<Material> Material;
    U::CheckedPtr<Mesh> Mesh;
    std::vector<M::Matrix4> ModelMatrices;
    std::vector<M::Matrix3> NormalMatrices;
    ArrayBuffer Buffer;

    RenderBatch() {
        Buffer.Usage = BufferUsage::DynamicDraw;
        Buffer.Generate();
    }
};
} // namespace N
