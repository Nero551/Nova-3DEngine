#include "ShaderSource.hpp"

#include <string>
#include <unordered_set>
#include <utility>

#include "Core/OuterCore/Resource.hpp"
#include "Modules/Renderer/ShaderSourceValidator.hpp"
#include "Utilities/FileSystem/FileSystem.hpp"
#include "Utilities/Logger.hpp"

namespace N {
ShaderSource::ShaderSource(const std::string& name, const std::string& path, const ShaderStage stage, std::string version) :
    Resource(name), Path(path), Version(std::move(version)), Stage(stage) {
    SourceCode = U::FileSystem::ReadFile(path);
}

ShaderSource::~ShaderSource() {
    glDeleteShader(Id);
}

unsigned int ShaderSource::GetId() const {
    return Id;
}

ShaderStage ShaderSource::GetStage() const {
    return Stage;
}

void ShaderSource::Compile() {
    if (IsCompiled()) {
        return;
    }

    Preprocess();

    auto result = ShaderSourceValidator::Validate(*this);
    if (!result.Success) {
        U::Logger::Error("[VALIDATOR] ", result.Log);
        if (Stage == ShaderStage::Fragment) {
            U::FileSystem::WriteFile("Assets/ShaderCompileError.frag", GeneratedCode);
        }
        if (Stage == ShaderStage::Vertex) {
            U::FileSystem::WriteFile("Assets/ShaderCompileError.vert", GeneratedCode);
        }
    }

    const char* string = GeneratedCode.c_str();

    Id = glCreateShader(static_cast<GLenum>(Stage));
    glShaderSource(Id, 1, &string, nullptr);
    glCompileShader(Id);

    int success;
    char infoLog[512];
    glGetShaderiv(Id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(Id, 512, nullptr, infoLog);
        U::Logger::Error(std::string("Shader:" + Name) + infoLog + " | " + Path);

        if (Stage == ShaderStage::Fragment) {
            U::FileSystem::WriteFile("Assets/ShaderCompileError.frag", GeneratedCode);
        }
        if (Stage == ShaderStage::Vertex) {
            U::FileSystem::WriteFile("Assets/ShaderCompileError.vert", GeneratedCode);
        }
        glDeleteShader(Id);
        Id = 0;
    }
}

bool ShaderSource::IsCompiled() const {
    return Id != 0;
}

void ShaderSource::Reload() {
    SourceCode = U::FileSystem::ReadFile(Path);
    glDeleteShader(Id);
    Id = 0;
}

void ShaderSource::Preprocess() {
    GeneratedCode = SourceCode;
    Includes.clear();
    GeneratedCode.insert(0, "#" + Version + "\n");

    std::unordered_set<std::string> includesProcessing;
    PreprocessIncludes(Path, GeneratedCode, includesProcessing);
}

// TODO- replace with line by line parsing
void ShaderSource::PreprocessIncludes(
    const std::string& path, std::string& code, std::unordered_set<std::string>& includesProcessing) {
    const std::string include = "#include \"";
    auto pos = code.find(include);

    while (pos != std::string::npos) {
        const auto start = pos + include.length();
        const auto end = code.find('\"', start);
        const auto directory = code.substr(start, end - start);
        auto includePath = std::filesystem::path(path).parent_path() / directory;

        if (!includePath.empty()) {
            // Check If Is Already Included
            if (Includes.contains(includePath)) {
                code.replace(pos, end - pos + 1, "");
            }
            else {
                // Check Circular Include
                if (!includesProcessing.insert(includePath).second) {
                    U::Logger::Fatal("Circular Include: " + includePath.string() + " | In Shader: " + path);
                }

                // Recursively Include
                std::string includeCode = U::FileSystem::ReadFile(includePath);
                PreprocessIncludes(includePath, includeCode, includesProcessing);

                code.replace(pos, end - pos + 1, includeCode);
                Includes.insert(includePath);

                includesProcessing.erase(includePath);
            }
        }
        pos = code.find(include, pos + 1);
    }
}
} // namespace N
