#pragma once

#include <glad/glad.h>

unsigned int LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);

void UniformVec3(unsigned int shader, const char* name, float x, float y, float z);

void UniformVec3v(unsigned int shader, const char* name, const GLfloat *val);

void UniformMat4v(unsigned int shader, const char* name, const GLfloat *val);
