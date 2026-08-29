#include "Includes/Default/Default.frag"
#include "Post-Processing/Post-Processing.frag"

uniform sampler2D colorbuffer;



void main() {
    FragColor = BlurKernel(colorbuffer, vUV, 2.0f / 300.0f) + EdgeDetectionKernel(colorbuffer, vUV, 2.0f / 300.0f);
}