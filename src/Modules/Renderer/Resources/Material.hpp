#pragma once

#include "Core/OuterCore/Resource.hpp"
#include "Math/Vector/Vector3.hpp"
#include "Math/Vector/Vector4.hpp"
#include "Modules/Renderer/Depth.hpp"
#include "Modules/Renderer/Stencil.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Utilities/CheckedPtr.hpp"

namespace N {
/**
 * @brief Represents the rendering properties and textures of a surface.
 *
 * A Material defines the shader, color properties, lighting properties,
 * and textures used when rendering an object. It also supports a fixed
 * number of custom texture slots.
 *
 * A default white texture is assigned to the standard material maps when
 * the material is created.
 */
struct Material : Resource {
    /** Maximum number of custom textures that can be assigned to a material. */
    static constexpr int MaxCustomTextures = 8;

    /** Shader used to render the material. */
    U::CheckedPtr<Shader> Shader{ "Material Has No Shader Assigned" };

    /** Base material color. */
    M::Vector4 Color = { 0.5 };

    /** Ambient lighting color. */
    M::Vector3 Ambient = { 0.7 };

    /** Diffuse lighting color. */
    M::Vector3 Diffuse = { 0.3 };

    /** Specular lighting color. */
    M::Vector3 Specular = { 0.7 };

    /** Emissive color of the material. */
    M::Vector3 Emission = { 0 };

    Stencil Stencil = false;
    Depth Depth = true;

    /** Specular shininess exponent. */
    float Shininess = 32;

    /** Texture used for the material's diffuse map. */
    U::CheckedPtr<Texture> DiffuseMap;

    /** Texture used for the material's specular map. */
    U::CheckedPtr<Texture> SpecularMap;

    /** Texture used for the material's emission map. */
    U::CheckedPtr<Texture> EmissionMap;

    /**
     * @brief Creates a material with default properties.
     *
     * Standard material maps are initialized to the default white texture.
     *
     * @param name Resource name.
     */
    Material(const std::string& name);

    /**
     * @brief Assigns a custom texture to a texture slot.
     * @param texture Texture to assign.
     * @param slot Custom texture slot to use.
     * @note Valid custom texture slots range from 0 to MaxCustomTextures - 1.
     *       An out-of-bounds slot is rejected and logged as an error.
     */
    void AssignTexture(Texture& texture, unsigned int slot);

    /**
     * @brief Activates the material and uploads its properties.
     * Uses the assigned shader, uploads material properties and standard
     * texture maps, then binds all assigned custom textures.
     */
    void Use();

protected:
    /** Textures assigned to the material's custom texture slots. */
    std::array<U::CheckedPtr<Texture>, MaxCustomTextures> CustomTextures = {};

    /**
     * @brief Uploads the material's properties and standard textures.
     * Called by Use() after the material's shader has been activated.
     */
    void SetProperties() const;
};
} // namespace N
