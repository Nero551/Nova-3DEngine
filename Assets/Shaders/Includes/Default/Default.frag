out vec4 FragColor;

in vec2 vUV;
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec4 vWorldPosition;

uniform vec3 VIEW_POSITION;
uniform float TIME;

struct Material {
    vec4 Color;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Emission;

    float Shininess;

    sampler2D DiffuseMap;
    sampler2D SpecularMap;
    sampler2D EmissionMap;
};

uniform Material MATERIAL;

void CheckAlpha(){
    if (FragColor.a < 0.1)
    discard;
}
