#include "Includes/Lighting/Lighting.frag"


uniform sampler2D grassTexture;

void main()
{
    FragColor = texture(grassTexture, vUV);
    CheckAlpha();
}
