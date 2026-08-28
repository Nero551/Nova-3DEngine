#pragma once
#include "Core/OuterCore/Resource.hpp"
#include "OpenGL.hpp"

namespace N {
/**
 * @brief Represents an OpenGL Uniform Buffer Object.
 */
struct Uniformbuffer : Resource {
    /** @brief Size of the buffer in bytes. */
    unsigned int Size = 0;

    /** @brief Uniform buffer binding point. */
    unsigned int Binding = 0;

    Uniformbuffer(const std::string& name) : Resource(name) {
    }

    /** @brief Generates the OpenGL uniform buffer. */
    void Generate();

    /**
     * @brief Writes a value to the buffer at the specified byte offset.
     * @param value Value to write.
     * @param offset Byte offset within the buffer.
     */
    template <typename T> void Set(const T& value, int offset) {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &value);
        Unbind();
    }

    /** @brief Checks whether the uniform buffer has been generated. */
    bool IsGenerated() const;

    /** @brief Deletes the OpenGL uniform buffer. */
    void Regenerate();

    /** @brief Binds the uniform buffer to its binding point. */
    void Bind();

    /** @brief Unbinds the uniform buffer from its binding point. */
    void Unbind();

    /** @brief Gets the OpenGL uniform buffer object ID. */
    unsigned int GetId() const;

private:
    unsigned int Id = 0;
};
} // namespace N
