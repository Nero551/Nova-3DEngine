layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec4 Position;
    vec4 WorldPosition;
    vec4 Color;
    vec3 Normal;
    vec2 UV;
    vec3 UVW;
} GSIn[];

out VS_OUT {
    vec4 Position;
    vec4 WorldPosition;
    vec4 Color;
    vec3 Normal;
    vec2 UV;
    vec3 UVW;
} GSOut;


void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        GSOut.Position = GSIn[i].Position;
        GSOut.WorldPosition = GSIn[i].WorldPosition;
        GSOut.Color = GSIn[i].Color;
        GSOut.UV = GSIn[i].UV;
        GSOut.Normal = GSIn[i].Normal;
        GSOut.UVW = GSIn[i].UVW;

        EmitVertex();
    }

    EndPrimitive();
}