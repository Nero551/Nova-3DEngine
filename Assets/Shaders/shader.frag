#include "Includes/Lighting/Lighting.frag"


uniform sampler2D snowflake;

void main()
{
    FragColor = vec4(Lighting(), 1) * MATERIAL.Color + texture(snowflake, vUV);
}
