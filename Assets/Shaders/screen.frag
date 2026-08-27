#include "Includes/Default/Default.frag"
#include "Post-Processing/Post-Processing.frag"

uniform sampler2D colorbuffer;



void main() {
    FragColor = texture(colorbuffer, vUV);
}