#include "Includes/Lighting/Lighting.frag"


uniform samplerCube cubemap;

void main()
{
    FragColor = texture(cubemap, vUVW);
}
