#include "Includes/Default/Default.frag"

uniform sampler2D colorbuffer;

vec4 Inversion(){
    return 1 - texture(colorbuffer, vUV);
}

vec4 Grayscale(){
    vec4 color = texture(colorbuffer, vUV);
    float red = 0.2126 * color.r;
    float green = 0.8152 * color.g;
    float blue = 0.0722 * color.b;
    float average = red + green + blue;
    return vec4(average, average, average, 1);
}

vec4 Kernel(float kernel[9], float offset){
    vec2 offsets[9] = vec2[](
            vec2(-offset, offset), // top-left
            vec2(0.0f,
                    offset), // top-center
            vec2(offset, offset), // top-right
            vec2(-offset, 0.0f),
            // center-left
            vec2(0.0f,
                    0.0f),
            // center-center
            vec2(offset, 0.0f),
            // center-right
            vec2(-offset, -offset), // bottom-left
            vec2(0.0f,
                    -offset), // bottom-center
            vec2(offset, -offset) // bottom-right
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(colorbuffer, vUV.st +
        offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];
    return vec4(col, 1.0);
}

vec4 SharpenKernel(float offset){
    float kernel[9] = float[](
            -1, -1, -1,
            -1, 9, -1,
            -1, -1, -1
    );
    return Kernel(kernel, offset);
}

vec4 BlurKernel(float offset){
    float kernel[9] = float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
    );
    return Kernel(kernel, offset);
}

vec4 EdgeDetectionKernel(float offset){
    float kernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
    );
    return Kernel(kernel, offset);
}

vec4 Pixelated(vec2 pixelSize, vec2 resolution){
    vec2 normalizedPixelSize = pixelSize / resolution;
    vec2 uvPixel = normalizedPixelSize * floor(vUV / normalizedPixelSize);

    return texture(colorbuffer, uvPixel);
}


void main()
{
    FragColor = Pixelated(vec2(8, 8), vec2(800, 600));
}
