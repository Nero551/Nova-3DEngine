#pragma once
#include "Math/Matrix/Matrix4.hpp"
#include "Resources/Material/Material.hpp"
#include "Resources/Mesh/Mesh.hpp"

namespace N
{
struct InstanceData
{
    M::Matrix4 ModelMatrix;
    M::Matrix3 NormalMatrix;

    InstanceData(const M::Matrix4& model, const M::Matrix3& normal) : ModelMatrix(model), NormalMatrix(normal)
    {
    }
};

struct RenderBatch
{
    U::CheckedPtr<Material> Material;
    U::CheckedPtr<Mesh> Mesh;
    std::vector<InstanceData> Instances;
    ArrayBuffer Buffer;
    VertexArray VAO;

    RenderBatch(const U::CheckedPtr<struct Mesh>& mesh, const U::CheckedPtr<struct Material>& mat)
        : Material(mat), Mesh(mesh)
    {
        Buffer.Usage = BufferUsage::DynamicDraw;
        Buffer.Generate();
    }

    void Render()
    {
        Material->Use();

        int instanceCount = Instances.size();
        Buffer.SetData(Instances);

        U::Logger::Info("Matrix4: " + std::to_string(sizeof(M::Matrix4)));
        U::Logger::Info("Matrix3: " + std::to_string(sizeof(M::Matrix3)));
        U::Logger::Info("InstanceData: " + std::to_string(sizeof(InstanceData)));
        U::Logger::Info("Matrix3 offset: " + std::to_string(offsetof(InstanceData, NormalMatrix)));

        Mesh->Generate();
        Mesh->VAO.SetVertexBuffer(Buffer, 1, sizeof(InstanceData));

        Mesh->VAO.SetMatrix4AttribPointer(4, 0, 1);
        Mesh->VAO.SetMatrix3AttribPointer(8, sizeof(M::Matrix4), 1);
        Mesh->VAO.SetAttribDivisor(1, 1);

        Mesh->DrawInstanced(instanceCount);
    }

    using BatchKey = std::pair<struct Material*, struct Mesh*>;
    struct BatchKeyHash
    {
        std::size_t operator()(const BatchKey& key) const
        {
            const std::size_t h1 = std::hash<struct Material*>{}(key.first);
            const std::size_t h2 = std::hash<struct Mesh*>{}(key.second);

            return h1 ^ (h2 << 1);
        }
    };
};
} // namespace N
