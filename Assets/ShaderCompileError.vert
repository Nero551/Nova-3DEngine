#version 450 core
layout (std140, binding = 0) uniform Global {
    mat4 VIEW_MATRIX;
    mat4 PROJECTION_MATRIX;
    float TIME;
    vec3 VIEW_POSITION;
};


layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec4 vPosition;
out vec4 vWorldPosition;
out vec4 vColor;
out vec2 vUV;
out vec3 vNormal;
out vec3 vUVW;

out VS_OUT {

} vs_out;

in VS_OUT {

} gs_in[];

uniform mat4 MODEL_MATRIX;
uniform mat3 NORMAL_MATRIX;

void DefaultVertex()
{
    vNormal = normalize(NORMAL_MATRIX * aNormal);
    vColor = aColor;
    vPosition = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * aPosition;
    vWorldPosition = MODEL_MATRIX * aPosition;
    vUV = aUV;
    vUVW = normalize(aPosition.xyz);
}


void main() {
    vNormal = normalize(aNormal);
    vColor = aColor;
    vPosition = aPosition;
    vWorldPosition = aPosition;
    vUV = aUV;
    gl_Position = vPosition;
}
