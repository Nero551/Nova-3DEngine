#include "Includes/Lighting/Lighting.frag"


uniform sampler2D grassTexture;

void main()
{
    FragColor = texture(grassTexture, vUV);
    if (FragColor.a < 0.1)
    discard;
}
