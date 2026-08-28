#pragma once
#include <glad/glad.h>

#include "Core/OuterCore/Resource.hpp"

namespace N {
struct Uniformbuffer : Resource {
    unsigned int Size = 0;
    unsigned int Binding = 0;

    Uniformbuffer(const std::string& name) : Resource(name) {
    }

    void Generate();

    template <typename T> void Set(const T& value, int offset) {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &value);
        Unbind();
    }

    bool IsGenerated() const;

    void Regenerate();

    void Bind();

    void Unbind();

    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
