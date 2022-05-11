#version 330 core

out vec4 FragColor;
uniform vec3 color;
uniform vec3 CameraPos;
uniform float dist;

in vec3 FragPos;

void main()
{
    vec3 fragColor = color;
    if (FragPos.x == 0.0) {
        fragColor = vec3(1.0, 0.0, 0.0);
    }
    if (FragPos.z == 0.0) {
        fragColor = vec3(0.0, 0.0, 1.0);
    }
    float power = 1.2;
    vec3 backgroundColor = vec3(0.1);
    float falloff = max(pow(min(1/((1/dist)*length(CameraPos - FragPos)), 1), power) * 1.2 - 0.2, 0);
    FragColor = vec4(fragColor, falloff);
    //FragColor = vec4(vec3(1.0) * clamp(1 / (length(FragPos) * 0.2), 0.0, 1.0), 1.0f);
}
