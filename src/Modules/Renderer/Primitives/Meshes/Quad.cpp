#include "../Primitives.hpp"
#include "Core/Services/ResourceManager.hpp"

namespace N
{
Mesh& Primitives::CreateQuad(const std::string& name)
{
    if (Service::Get<ResourceManager>().Exists<Mesh>(name))
    {
        return Service::Get<ResourceManager>().Load<Mesh>(name);
    }

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f, 1.0f}, {1, 1, 1, 1}, {0, 0}, {0, 0, 1}},
        {{0.5f, -0.5f, 0.0f, 1.0f},  {1, 1, 1, 1}, {1, 0}, {0, 0, 1}},
        {{0.5f, 0.5f, 0.0f, 1.0f},   {1, 1, 1, 1}, {1, 1}, {0, 0, 1}},
        {{-0.5f, 0.5f, 0.0f, 1.0f},  {1, 1, 1, 1}, {0, 1}, {0, 0, 1}}
    };

    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    auto& mesh = Service::Get<ResourceManager>().Load<Mesh>(name);
    mesh.Vertices = std::move(vertices);
    mesh.Indices = std::move(indices);

    return mesh;
}
} // namespace N