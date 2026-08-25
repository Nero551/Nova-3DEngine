#include "../Primitives.hpp"
#include "Core/Services/ResourceManager.hpp"

namespace N {
Mesh& Primitives::CreateQuad(const std::string& name) {
    std::vector<Vertex> vertices = {
        { { -0.5f, -0.5f, 0.0f, 1.0f }, { 1, 1, 1, 1 }, { 0, 0 }, { 0, 0, 1 } },
        { { 0.5f, -0.5f, 0.0f, 1.0f }, { 1, 1, 1, 1 }, { 1, 0 }, { 0, 0, 1 } },
        { { 0.5f, 0.5f, 0.0f, 1.0f }, { 1, 1, 1, 1 }, { 1, 1 }, { 0, 0, 1 } },
        { { -0.5f, 0.5f, 0.0f, 1.0f }, { 1, 1, 1, 1 }, { 0, 1 }, { 0, 0, 1 } },
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

    auto& mesh = Service::Get<ResourceManager>().Load<Mesh>(name, vertices, indices);

    return mesh;
}
} // namespace N
