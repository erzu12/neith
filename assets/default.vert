
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 4) in mat4 aModel;

out vec3 pos;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    pos = aPos;
    //gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z , 1.0f);
    gl_Position = projection * view * aModel * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}
