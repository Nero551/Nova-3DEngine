#pragma once
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"

namespace N {
struct InstanceData {
    M::Matrix4 ModelMatrix;
    M::Matrix3 NormalMatrix;

    InstanceData(const M::Matrix4& model, const M::Matrix3& normal) : ModelMatrix(model), NormalMatrix(normal) {
    }
};

struct RenderBatch {
    U::CheckedPtr<Material> Material;
    U::CheckedPtr<Mesh> Mesh;
    std::vector<InstanceData> Instances;
    ArrayBuffer Buffer;

    RenderBatch(const U::CheckedPtr<struct Mesh>& mesh, const U::CheckedPtr<struct Material>& mat) : Material(mat), Mesh(mesh) {
        Buffer.Usage = BufferUsage::DynamicDraw;
        Buffer.Generate();
    }
};
} // namespace N
