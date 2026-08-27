#include "Mesh.hpp"

#include <OpenGL.hpp>

namespace N {
Mesh::Mesh(const std::string& name) : Resource(name) {
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &Id);
}

unsigned int Mesh::GetId() const {
    return Id;
}

bool Mesh::IsGenerated() const {
    return GetId() != 0;
}

void Mesh::Generate() {
    if (IsGenerated()) {
        return;
    }
    CreateVAO();
    CreateVBO();
    CreateEBO();
    SetupVertAttrPointers();
    glBindVertexArray(0);
}

void Mesh::Draw() {
    Generate();

    ApplyCulling();
    glBindVertexArray(Id);
    if (RenderMode == RenderMode::Solid) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        DrawElements();
    }

    else if (RenderMode == RenderMode::Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        DrawElements();
    }

    else if (RenderMode == RenderMode::SolidWireframe) {
        // TODO- apparently there is a better way to do this using geometry shaders (search SolidWireframe opengl on yt).
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        DrawElements();

        glDepthFunc(GL_ALWAYS);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        DrawElements();

        glDepthFunc(GL_LESS);
    }
    glBindVertexArray(0);
}

void Mesh::Regenerate() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &Id);
    Id = 0;
}

void Mesh::ApplyCulling() const {
    glFrontFace(static_cast<GLenum>(FrontFace));

    if (CullMode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    }
    else {
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<GLenum>(CullMode));
    }
}

void Mesh::DrawElements() const {
    glDrawElements(static_cast<GLenum>(Topology), static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);
}

void Mesh::CreateVAO() {
    glGenVertexArrays(1, &Id);
    glBindVertexArray(Id);
}

void Mesh::CreateVBO() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);
}

void Mesh::CreateEBO() {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);
}

void Mesh::SetupVertAttrPointers() {
    // Position
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));
    glEnableVertexAttribArray(2);

    // Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    glEnableVertexAttribArray(3);
}
} // namespace N
