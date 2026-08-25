#include "../Primitives.hpp"
#include "Core/Services/ResourceManager.hpp"
#include "Math/Color/Color.hpp"

namespace N {
Mesh& Primitives::CreateUVSphere(const std::string& name, const float radius, const int sectors, const int stacks) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int stack = 0; stack <= stacks; stack++) {
        float phi = std::numbers::pi * stack / stacks;

        float y = cos(phi);
        float r = sin(phi);

        for (int sector = 0; sector <= sectors; sector++) {
            float theta = 2.0f * std::numbers::pi * sector / sectors;

            float x = r * std::cos(theta);
            float z = r * std::sin(theta);

            vertices.emplace_back(Vertex{ { radius * x, radius * y, radius * z, 1.0f },
                M::Color::White,
                { sector / static_cast<float>(sectors), stack / static_cast<float>(stacks) },
                { x, y, z } });
        }
    }

    for (int stack = 0; stack < stacks; stack++) {
        int k1 = stack * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int sector = 0; sector < sectors; sector++) {
            if (stack != 0) {
                indices.insert(
                    indices.end(), { static_cast<unsigned>(k1), static_cast<unsigned>(k2), static_cast<unsigned>(k1 + 1) });
            }

            if (stack != stacks - 1) {
                indices.insert(
                    indices.end(), { static_cast<unsigned>(k1 + 1), static_cast<unsigned>(k2), static_cast<unsigned>(k2 + 1) });
            }

            k1++;
            k2++;
        }
    }

    auto& mesh = Service::Get<ResourceManager>().Load<Mesh>(name, vertices, indices);
    return mesh;
}
} // namespace N
