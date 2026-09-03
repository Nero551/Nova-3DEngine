#include "Shader.hpp"

#include <OpenGL.hpp>

#include "Math/Complex/Logarithms.hpp"
#include "Uniforms/FloatUniform.hpp"
#include "Utilities/Logger.hpp"

namespace N {
Shader::Shader(const std::string& name) : Resource(name) {
}

Shader::~Shader() {
    glDeleteProgram(Id);
}

unsigned int Shader::GetId() const {
    return Id;
}

void Shader::Use() {
    CreateProgram();
    glUseProgram(Id);
    UploadUniforms();
}

bool Shader::IsProgramCreated() const {
    return Id != 0;
}

void Shader::AssignSource(ShaderSource& source) {
    for (auto& existing : Sources) {
        if (existing->GetStage() == source.GetStage()) {
            // N::U::Logger::Error("Shader: " + Name + " Duplicate Shader Stage.");
            return;
        }
    }

    Sources.emplace_back(&source);
}

void Shader::Reload() {
    for (auto& source : Sources) {
        source->Reload();
    }

    UniformLocations.clear();
    glDeleteProgram(Id);
    Id = 0;
}

std::vector<U::CheckedPtr<ShaderSource>>& Shader::GetSources() {
    return Sources;
}

void Shader::CreateProgram() {
    if (IsProgramCreated()) {
        return;
    }

    // TODO- add preprocessing to shader program, ex: a ShowNormals option that when true, automatically adds a geometry shader
    //  for that.
    //  gotta figure out how to "Merge" multiple sources too, like if there is 2 vertex shaders attached both with void main().
    //  Preprocess();


    if (Sources.empty()) {
        U::Logger::Warning("Shader Program:" + Name + " Has No Sources");
        return;
    }

    Id = glCreateProgram();

    for (const auto& source : Sources) {
        if (!source->IsCompiled()) {
            source->Compile();
        }
        glAttachShader(Id, source->GetId());
    }
    glLinkProgram(Id);

    int success;
    char infoLog[512];
    glGetProgramiv(Id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Id, 512, nullptr, infoLog);
        U::Logger::Error(std::string("Shader Program: " + Name + " Linking Failed: ") + infoLog);
    }
}

int Shader::GetUniformLocation(const std::string& name) {
    int location;

    if (UniformLocations.contains(name)) {
        location = UniformLocations[name];
    }
    else {
        location = glGetUniformLocation(Id, name.c_str());

        if (location == -1) {
            // U::Logger::Warning("Shader: " + Name + " Uniform Not Found: " + name);
            return -1;
        }

        UniformLocations[name] = location;
    }
    return location;
}

void Shader::UploadUniforms() {
    for (auto& [uniformName, uniform] : PendingUniforms) {
        int location = GetUniformLocation(uniformName);
        if (location != -1) {
            uniform->Upload(location);
        }
    }
}
} // namespace N
