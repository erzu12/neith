#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 aModel;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * aModel * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
}

