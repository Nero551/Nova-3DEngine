#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "CullMode.hpp"
#include "FrontFace.hpp"
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
     * Determines which faces are discarded during rasterization.
     */
    CullMode CullMode = CullMode::Back;

    /**
     * @brief Winding order considered front-facing.
     * Determines how OpenGL identifies front-facing and back-facing faces.
     */
    FrontFace FrontFace = FrontFace::CounterClockwise;

    /** CPU-side vertex data used to generate the GPU resources. */
    std::vector<Vertex> Vertices;

    /** CPU-side index data used to generate the GPU resources. */
    std::vector<unsigned int> Indices;

    /**
     * @brief Creates a mesh from vertex and index data.
     *
     * @param name Resource name.
     */
    Mesh(const std::string& name);

    /** Releases the OpenGL resources owned by the mesh. */
    ~Mesh() override;

    /**
     * @brief Gets the OpenGL vertex array object ID.
     * @return OpenGL VAO ID, or 0 if the mesh has not been generated.
     */
    [[nodiscard]] unsigned int GetId() const;

    /** @return Whether the mesh's OpenGL resources have been generated. */
    bool IsGenerated() const;

    /**
     * @brief Generates the OpenGL resources required to render the mesh.
     *
     * Creates the VAO, VBO, EBO, and vertex attribute configuration from
     * the mesh's stored vertex and index data.
     */
    void Generate();

    /**
     * @brief Draws the mesh using its configured rendering state.
     *
     * Applies the culling mode, front-face winding, render mode, and
     * topology before issuing the draw call. Generates the OpenGL
     * resources first if they have not yet been created.
     */
    void Draw();

    /**
     * @brief Regenerates the mesh's OpenGL resources.
     *
     * Deletes the existing GPU resources and marks the mesh as not
     * generated so they can be recreated by Generate().
     */
    void Regenerate();

private:
    /** OpenGL vertex array object ID. */
    unsigned int Id = 0;

    /** OpenGL vertex buffer object ID. */
    unsigned int VBO = 0;

    /** OpenGL element buffer object ID. */
    unsigned int EBO = 0;

    /** Issues the OpenGL draw call for the mesh's index data. */
    void DrawElements() const;

    /**
     * @brief Applies the mesh's face culling configuration.
     *
     * Configures the front-face winding and enables or disables face
     * culling according to the mesh's CullMode and FrontFace settings.
     */
    void ApplyCulling() const;

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
