#version 330 core
uniform sampler2D alpha;

in vec2 texCoord;

void main() {
    if(texture(alpha, texCoord).r < 0.7)
        discard;
}
