#version 330 core

layout (location = 0) in vec3 aPos;
uniform mat4 VP;
uniform mat4 model;

out vec3 FragPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    mat4 MVP = VP * model;
    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
