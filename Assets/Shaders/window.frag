#include "Includes/Default/Default.frag"

uniform sampler2D windowTexture;

void main()
{
    FragColor = texture(windowTexture, vUV);
    CheckAlpha();
}
