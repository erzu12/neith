#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

namespace neith {
unsigned int LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);
char* LoadShader(const char* path);
void UniformVec3(unsigned int shader, const char* name, float x, float y, float z);
void UniformVec3v(unsigned int shader, const char* name, const GLfloat* val);
void UniformMat4v(unsigned int shader, const char* name, const glm::mat4& val);
}  // namespace neith
