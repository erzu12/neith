#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
	//FragColor = texture(skybox, TexCoords);
	FragColor = vec4(0.05, 0.05, 0.05, 1.0);
}
