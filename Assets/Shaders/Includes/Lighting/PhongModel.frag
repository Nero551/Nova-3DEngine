#include "../Default/Default.frag"

struct Light {
    int Type;
    vec3 Direction;
    vec3 Color;
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    float Intensity;
    float Constant;
    float Linear;
    float Quadratic;

    float InnerCutOff;
    float OuterCutOff;
};

vec3 CalculateAmbient(Light light){
    vec3 diffuseMap = vec3(texture(MATERIAL.DiffuseMap, fs_in.UV));
    vec3 ambient = light.Color * diffuseMap * light.Ambient * MATERIAL.Ambient * light.Intensity;
    return ambient;
}

vec3 CalculateDiffuse(Light light, vec3 lightDir, float attenuation, float cutOff){
    vec3 diffuseMap = vec3(texture(MATERIAL.DiffuseMap, fs_in.UV));
    float diff = max(dot(fs_in.Normal, lightDir), 0.0);
    vec3 diffuse = diff * light.Color * diffuseMap * light.Diffuse * MATERIAL.Diffuse * attenuation * cutOff * light.Intensity;
    return diffuse;
}

vec3 CalculateSpecular(Light light, vec3 lightDir, float attenuation, float cutOff){
    vec3 specularMap = vec3(texture(MATERIAL.SpecularMap, fs_in.UV));
    vec3 viewDir = normalize(VIEW_POSITION - vec3(fs_in.WorldPosition.xyz));
    vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL.Shininess);
    vec3 specular = spec * light.Color * specularMap * MATERIAL.Specular * light.Specular * attenuation * cutOff * light.Intensity;

    return specular;
}

vec3 CalculateEmission(){
    vec3 emissionMap = vec3(texture(MATERIAL.EmissionMap, fs_in.UV));
    vec3 emission = emissionMap * MATERIAL.Emission;
    return emission;
}
