#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "CullMode.hpp"
#include "RenderMode.hpp"
#include "Topology.hpp"
#include "Vertex.hpp"

namespace N {
/**
 * @brief Represents a renderable mesh resource.
 *
 * Stores vertex and index data and manages the OpenGL resources required
 * to render the mesh. GPU resources are generated lazily when Generate()
 * is called.
 */
struct Mesh : Resource {
    /** Rendering mode used when drawing the mesh. */
    RenderMode RenderMode = RenderMode::Solid;

    /** Primitive topology used to interpret the mesh indices. */
    Topology Topology = Topology::Triangles;

    /**
     * @brief Face culling mode used when drawing the mesh.
     * Determines whether front-facing, back-facing, or both types of faces
     * are discarded during rasterization.
     */
    CullMode CullMode = CullMode::Front;

    /** CPU-side vertex data used to generate the GPU resources. */
    std::vector<Vertex> Vertices;

    /** CPU-side index data used to generate the GPU resources. */
    std::vector<unsigned int> Indices;

    /**
     * @brief Creates a mesh from vertex and index data.
     *
     * @param name Resource name.
     * @param vertices Vertex data used to construct the mesh.
     * @param indices Index data used to construct the mesh.
     */
    Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    /** Releases the OpenGL resources owned by the mesh. */
    ~Mesh() override;

    /**
     * @brief Gets the OpenGL vertex array object ID.
     * @return OpenGL VAO ID, or 0 if the mesh has not been generated.
     */
    [[nodiscard]] unsigned int GetId() const;

    /**
     * @brief Generates the OpenGL resources required to render the mesh.
     * Creates the vertex array, vertex buffer, element buffer, and vertex attribute
     * configuration from the mesh's stored vertices and indices data.
     */
    bool IsGenerated() const;
    void Generate();

    /**
     * @brief Enables the configured face culling mode and Draws the mesh using its configured render mode and topology.
     *
     * Generates the OpenGL resources first if they have not yet been created.
     */
    void Draw();

private:
    /** OpenGL vertex array object ID. */
    unsigned int Id = 0;

    /** OpenGL vertex buffer object ID. */
    unsigned int VBO = 0;

    /** OpenGL element buffer object ID. */
    unsigned int EBO = 0;

    /** Creates and binds the vertex array object. */
    void CreateVAO();

    /** Creates and uploads the vertex buffer object. */
    void CreateVBO();

    /** Creates and uploads the element buffer object. */
    void CreateEBO();

    /** Configures the vertex attribute pointers. */
    void SetupVertAttrPointers();
};
} // namespace N
