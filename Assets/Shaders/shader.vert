#include "Includes/Default/Default.vert"

void main() {
    DefaultVertex();
    gl_Position = vs_out.Position;
}
