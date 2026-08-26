#version 330 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec4 vPosition;
out vec4 vWorldPosition;
out vec4 vColor;
out vec2 vUV;
out vec3 vNormal;

uniform mat4 MODEL_MATRIX;
uniform mat4 VIEW_MATRIX;
uniform mat4 PROJECTION_MATRIX;
uniform mat3 NORMAL_MATRIX;

uniform vec3 VIEW_POSITION;
uniform float TIME;

void DefaultVertex()
{
    vNormal = normalize(NORMAL_MATRIX * aNormal);
    vColor = aColor;
    vPosition = MODEL_MATRIX * VIEW_MATRIX * PROJECTION_MATRIX aPosition;
    vWorldPosition = MODEL_MATRIX * aPosition;
    vUV = aUV;
}


void main() {
    DefaultVertex();
    gl_Position = vPosition;
}
