#include "../Primitives.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"

namespace N {
Mesh& Primitives::CreateLine(const std::string& name) {
    std::vector vertices = { Vertex({ 0, 0, -0.5f, 1 }, { 1 }, { 0 }, { 0, 0, 1 }),
        Vertex({ 0, 0, 0.5f, 1 }, { 1 }, { 0 }, { 0, 0, 1 }) };

    std::vector<unsigned int> indices = { 0, 1 };

    Mesh& mesh = Service::Get<ResourceManager>().Load<Mesh>(name, vertices, indices);
    mesh.Topology = Topology::Lines;
    return mesh;
}
} // namespace N
