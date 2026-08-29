#include "../GlobalUniforms.glsl"

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out VS_OUT {
    vec4 Position;
    vec4 WorldPosition;
    vec4 Color;
    vec3 Normal;
    vec2 UV;
    vec3 UVW;

} vs_out;

uniform mat4 MODEL_MATRIX;
uniform mat3 NORMAL_MATRIX;

void DefaultVertex()
{
    vs_out.Normal = normalize(NORMAL_MATRIX * aNormal);
    vs_out.Color = aColor;
    vs_out.Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * aPosition;
    vs_out.WorldPosition = MODEL_MATRIX * aPosition;
    vs_out.UV = aUV;
    vs_out.UVW = normalize(aPosition.xyz);
}
