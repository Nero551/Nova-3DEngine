#include "Includes/Default/Default.vert"

void main() {
    vNormal = normalize(aNormal);
    vColor = aColor;
    vPosition = aPosition;
    vWorldPosition = aPosition;
    vUV = aUV;
    gl_Position = vPosition;
}
