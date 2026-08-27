#include "Includes/Default/Default.frag"

uniform sampler2D colorbuffer;

void main()
{
    FragColor = texture(colorbuffer, vUV);
}
