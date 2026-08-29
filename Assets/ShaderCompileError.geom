#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec4 vPosition;
out vec4 vWorldPosition;
out vec4 vColor;
out vec2 vUV;
out vec3 vNormal;
out vec3 vUVW;
//
in vec2 vUV[];
in vec4 vPosition[];
in vec4 vColor[];
in vec3 vNormal[];
in vec4 vWorldPosition[];
in vec3 vUVW[];
//

//out VS_OUT {

//} vs_out;

//in VS_OUT {

//} gs_in[];

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        //
        vPosition = vPosition[i];
        vWorldPosition = vWorldPosition[i];
        vColor = vColor[i];
        vUV = vUV[i];
        vNormal = vNormal[i];
        vUVW = vUVW[i];

        EmitVertex();
    }

    EndPrimitive();
}