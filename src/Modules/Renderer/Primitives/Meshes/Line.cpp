#include "../Primitives.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"

namespace N
{
Mesh& Primitives::CreateLine(const std::string& name)
{
    if (Service::Get<ResourceManager>().Exists<Mesh>(name))
    {
        return Service::Get<ResourceManager>().Load<Mesh>(name);
    }

    std::vector vertices = {Vertex({0, 0, -0.5f, 1}, {1}, M::Vector2{0}, {0, 0, 1}),
        Vertex({0, 0, 0.5f, 1}, {1}, M::Vector2{0}, {0, 0, 1})};

    std::vector<unsigned int> indices = {0, 1};

    Mesh& mesh = Service::Get<ResourceManager>().Load<Mesh>(name);
    mesh.Vertices = std::move(vertices);
    mesh.Indices = std::move(indices);
    mesh.Topology = Topology::Lines;

    return mesh;
}
} // namespace N