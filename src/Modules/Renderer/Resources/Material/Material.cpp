#include "Material.hpp"

#include "../../Primitives/Primitives.hpp"
#include "../Shader/Uniforms/FloatUniform.hpp"
#include "../Shader/Uniforms/IntUniform.hpp"
#include "../Shader/Uniforms/Vector3Uniform.hpp"
#include "../Shader/Uniforms/Vector4Uniform.hpp"
#include "Utilities/Logger.hpp"

namespace N {
Material::Material(const std::string& name) : Resource(name) {
    auto& whiteTexture = Primitives::CreateWhiteTexture();

    DiffuseMap = &whiteTexture;
    SpecularMap = &whiteTexture;
    EmissionMap = &whiteTexture;
}

void Material::AssignTexture(Texture& texture, const unsigned int slot) {
    if (slot >= MaxCustomTextures) {
        U::Logger::Error("Material: ", Name, " Texture slot out of bounds: " + texture.Name);
        return;
    }
    CustomTextures[slot] = &texture;
}

void Material::SetProperties() const {
    Shader->SetUniform(Vector3Uniform("MATERIAL.Ambient", Ambient));
    Shader->SetUniform(Vector3Uniform("MATERIAL.Diffuse", Diffuse));
    Shader->SetUniform(Vector3Uniform("MATERIAL.Specular", Specular));
    Shader->SetUniform(Vector3Uniform("MATERIAL.Emission", Emission));
    Shader->SetUniform(FloatUniform("MATERIAL.Shininess", Shininess));
    Shader->SetUniform(Vector4Uniform("MATERIAL.Color", Color));

    Shader->SetUniform(IntUniform("MATERIAL.DiffuseMap", 16));
    DiffuseMap->Bind(16);

    Shader->SetUniform(IntUniform("MATERIAL.SpecularMap", 15));
    SpecularMap->Bind(15);

    Shader->SetUniform(IntUniform("MATERIAL.EmissionMap", 14));
    EmissionMap->Bind(14);
}

void Material::Use() {
    SetProperties();

    Depth.Apply();
    Stencil.Apply();
    Blend.Apply();

    Shader->Use();

    for (int slot = 0; slot < MaxCustomTextures; slot++) {
        if (CustomTextures[slot]) {
            Shader->SetUniform(IntUniform(CustomTextures[slot]->Name, slot));
            CustomTextures[slot]->Bind(slot);
        }
    }
}
} // namespace N
