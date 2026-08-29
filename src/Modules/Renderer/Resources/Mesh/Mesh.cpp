#include "Mesh.hpp"

#include <OpenGL.hpp>

namespace N {
Mesh::Mesh(const std::string& name) : Resource(name) {
}

Mesh::~Mesh() {
}

unsigned int Mesh::GetId() const {
    return VAO.GetId();
}

bool Mesh::IsGenerated() const {
    return VAO.GetId() != 0;
}

void Mesh::Generate() {
    if (IsGenerated()) {
        return;
    }
    VAO.Generate(Vertices, Indices);
    VAO.Bind();

    VAO.SetAttribPointer(0, 4, DataType::Float, sizeof(Vertex), offsetof(Vertex, Position));
    VAO.SetAttribPointer(1, 4, DataType::Float, sizeof(Vertex), offsetof(Vertex, Color));
    VAO.SetAttribPointer(2, 2, DataType::Float, sizeof(Vertex), offsetof(Vertex, UV));
    VAO.SetAttribPointer(3, 3, DataType::Float, sizeof(Vertex), offsetof(Vertex, Normal));

    VAO.Unbind();
}

void Mesh::Draw() {
    Generate();

    ApplyCulling();
    VAO.Bind();
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
    VAO.Unbind();
}

void Mesh::DrawInstanced(int instanceCount) {
    Generate();

    ApplyCulling();
    VAO.Bind();

    glDrawElementsInstanced(static_cast<GLenum>(Topology),
        static_cast<GLsizei>(Indices.size()),
        GL_UNSIGNED_INT,
        nullptr,
        static_cast<GLsizei>(instanceCount));

    VAO.Unbind();
}

void Mesh::Regenerate() {
    VAO.Delete();
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
} // namespace N
