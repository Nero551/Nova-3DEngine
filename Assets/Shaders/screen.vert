#include "Includes/Default/Default.vert"

void main() {
    vs_out.Normal = normalize(aNormal);
    vs_out.Color = aColor;
    vs_out.Position = aPosition;
    vs_out.WorldPosition = aPosition;
    vs_out.UV = aUV;
    gl_Position = vs_out.Position;
}
