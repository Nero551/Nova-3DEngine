layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec4 Position;
    vec4 WorldPosition;
    vec4 Color;
    vec3 Normal;
    vec2 UV;
    vec3 UVW;
} gs_in[];

out VS_OUT {
    vec4 Position;
    vec4 WorldPosition;
    vec4 Color;
    vec3 Normal;
    vec2 UV;
    vec3 UVW;
} gs_out;


void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        gs_out.Position = gs_in[i].Position;
        gs_out.WorldPosition = gs_in[i].WorldPosition;
        gs_out.Color = gs_in[i].Color;
        gs_out.UV = gs_in[i].UV;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.UVW = gs_in[i].UVW;

        EmitVertex();
    }

    EndPrimitive();
}