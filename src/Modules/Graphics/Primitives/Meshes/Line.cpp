#include "../Primitives.hpp"
#include "Core/InnerCore/Engine.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"

namespace N::G
{
Mesh& Primitives::CreateLine(const std::string& name)
{
    if (C::Service::Get<C::ResourceManager>().Exists<Mesh>(name))
    {
        return C::Service::Get<C::ResourceManager>().Load<Mesh>(name);
    }

    std::vector vertices = {Vertex({0, 0, -0.5f, 1}, M::Vector4{1}, M::Vector2{0}, {0, 0, 1}),
        Vertex({0, 0, 0.5f, 1}, M::Vector4{1}, M::Vector2{0}, {0, 0, 1})};

    std::vector<unsigned int> indices = {0, 1};

    Mesh& mesh = C::Service::Get<C::ResourceManager>().Load<Mesh>(name);
    mesh.Vertices = std::move(vertices);
    mesh.Indices = std::move(indices);
    mesh.Topology = Topology::Lines;

    return mesh;
}
} // namespace N::G