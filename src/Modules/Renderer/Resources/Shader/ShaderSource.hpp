#pragma once

#include <string>
#include <unordered_set>

#include "Core/OuterCore/Resource.hpp"
#include "ShaderStage.hpp"

namespace N {
/**
 * @brief Represents a shader source file and its OpenGL shader object.
 * Handles loading, preprocessing, and reloading of a shader source.
 */
struct ShaderSource : Resource {
    std::string Path;

    /** @brief code loaded from the source , no modifications */
    std::string SourceCode;

    /** @brief SourceCode with modifications (preprocessing) */
    std::string GeneratedCode;

    std::string Version = "version 450 core";

    // TODO- it would be really cool if i could have methods for adding code
    //  dynamically , like GenerateInt("MaxLights", 20)
    //  and it adds it to the shader code, (hint: dont forget std::variants)

    ShaderSource(const std::string& name, const std::string& path, ShaderStage stage, std::string version = "version 450 core");

    ~ShaderSource() override;
    unsigned int GetId() const;
    ShaderStage GetStage() const;

    /** @brief Preprocesses the source, generates an OpenGL Id & compiles the source */
    void Compile();

    /** @brief checks if the source was compiled.
     * @return true if compiled , false if not.
     */
    bool IsCompiled() const;

    /** @brief Reloads and preprocesses the shader source. */
    void Reload();

private:
    unsigned int Id = 0;
    ShaderStage Stage;

    /** @brief Paths of shader files included by this source. */
    std::unordered_set<std::string> Includes;

    void Preprocess();
    void PreprocessIncludes(const std::string& path, std::string& code, std::unordered_set<std::string>& includesProcessing);
};
} // namespace N
