#include "shaders.h"

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath) {

	FILE *fpVert, *fpFrag;
	unsigned int sizev, sizef;

	fpVert = fopen(vertexPath, "r");
	fpFrag = fopen(fragmentPath, "r");

	if(fpVert == NULL) {
		printf("faild to load %s\n", vertexPath);
		exit(EXIT_FAILURE);
	}
	if(fpFrag == NULL) {
		printf("faild to load %s\n", fragmentPath);
		exit(EXIT_FAILURE);
	}

	fseek(fpVert, 0, SEEK_END); 
	sizev = ftell(fpVert);
	fseek(fpVert, 0, SEEK_SET);

	fseek(fpFrag, 0, SEEK_END); 
	sizef = ftell(fpFrag);
	fseek(fpFrag, 0, SEEK_SET);

	char *vertCode = (char *)malloc((size_t)sizev + 1);
	if (vertCode == NULL) { printf("Memory error"); exit (EXIT_FAILURE); }
	char *fragCode = (char *)malloc((size_t)sizef + 1);
	if (fragCode == NULL) { printf("Memory error"); exit (EXIT_FAILURE); }

	int result = fread (vertCode, 1, sizev, fpVert);
	if (result != sizev) { printf("ERROR: faild to read: %s\n", vertexPath); }

	result = fread (fragCode, 1, sizef, fpFrag);
	if (result != sizef) { printf("ERROR: faild to read: %s\n", fragmentPath); }

	//for(int i = 0; i < sizev; i++){
	//	vertCode[i] = fgetc(fpVert);
	//}
	vertCode[sizev] = '\0';
	//for(int i = 0; i < sizef; i++){
	//	fragCode[i] = fgetc(fpFrag);
	//}
	fragCode[sizef] = '\0';

	fclose(fpVert);
	fclose(fpFrag);


	const char* vertCodeArr[1];
	const char* fragCodeArr[1];

	vertCodeArr[0] = vertCode;
	fragCodeArr[0] = fragCode;

	int success;
	char infoLog[512];

	//VertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, vertCodeArr, NULL);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR: VertexShader: %s faild to compile:\n%s\n", vertexPath, infoLog);
		exit(EXIT_FAILURE);
	}

	//Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(fragmentShader, 1, fragCodeArr, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR: FragmentShader: %s faild to compile:\n%s\n", fragmentPath, infoLog);
		exit(EXIT_FAILURE);
	}
	
	//Shader Program
	unsigned int shaderProgram;
	shaderProgram =glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR: shader_program linking faild", infoLog, "\n");
		exit(EXIT_FAILURE);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void UniformVec3(unsigned int shader, const char* name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(shader, name), x, y, z);
}
void UniformVec3v(unsigned int shader, const char* name, const GLfloat *val) {
	glUniform3fv(glGetUniformLocation(shader, name), 1, val);
}
void UniformMat4v(unsigned int shader, const char* name, const GLfloat *val) {
	glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, val);
}
