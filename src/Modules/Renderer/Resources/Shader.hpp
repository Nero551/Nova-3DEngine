#pragma once

#include "../Uniform.hpp"
#include "Core/OuterCore/Resource.hpp"
#include "ShaderSource.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
template <typename T>

/** @brief Concept for structs inheriting Uniform, used for OpenGL shader uniforms */
concept UniformType = std::derived_from<T, Uniform>;

/**
 * @brief Represents an OpenGL shader program.
 * manages the lifetime and use of an OpenGL shader program,
 * including its source files and pending uniform values.
 */
struct Shader : Resource {
    /**
     * @brief Whether the shader supports hot reloading.
     * When enabled, the shader can be reloaded when its source changes.
     */
    bool HotReload = false;

    /**
     * @brief Creates a shader resource.
     * @param name Name used to identify the shader resource.
     */
    Shader(const std::string& name);

    /** @brief Destroys the shader and releases its OpenGL program. */
    ~Shader() override;

    /**
     * @brief Gets the OpenGL shader program ID.
     * @return OpenGL program ID.
     */
    unsigned int GetId() const;

    /** @brief Activates the shader and uploads pending uniforms. */
    void Use();

    /** @brief Checks if the shader program exists.
     * @return  true if exists , false if not.
     */
    bool IsProgramCreated() const;

    /**
     * @brief Queues a uniform value for upload.
     *
     * @tparam T Uniform type derived from @ref Uniform.
     * @param uniform Uniform value to queue.
     *
     * @note The uniform is uploaded the next time the shader is used.
     */
    template <UniformType T> void SetUniform(const T& uniform) {
        PendingUniforms[uniform.Name] = std::make_unique<T>(uniform);
    }

    void AssignSource(ShaderSource& source);

    /** @brief deletes the current shader program and uniform locations , which
     * causes them to be recreated on Use() */
    void Reload();

    std::vector<U::CheckedPtr<ShaderSource>>& GetSources();

private:
    /** @brief OpenGL handle for the shader program. */
    unsigned int Id = 0;

    std::unordered_map<std::string, int> UniformLocations;

    /**
     * @brief Uniform values waiting to be uploaded to the GPU.
     * Uniforms are stored by name and resolved to OpenGL locations
     * when they are uploaded.
     */
    std::unordered_map<std::string, std::unique_ptr<Uniform>> PendingUniforms;

    std::vector<U::CheckedPtr<ShaderSource>> Sources;

    void CreateProgram();

    /**
     * @brief Gets and caches the OpenGL location of a uniform.
     * @param name Name of the shader uniform.
     * @return OpenGL uniform location.
     */
    int GetUniformLocation(const std::string& name);

    void UploadUniforms();
};
} // namespace N
