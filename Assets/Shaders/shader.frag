#include "Includes/Lighting/Lighting.frag"

in vec3 vertex_color;

void main()
{
    FragColor = vec4(vertex_color, 1);
}
