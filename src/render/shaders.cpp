#include "shaders.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <sstream>
#include <format>

#include "log.h"
#include "render/render.h"

namespace neith {

unsigned int CompileShader(const char* shaderPath, GLuint shaderType) {
    std::string computeCode;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        cShaderFile.open(shaderPath);
        std::stringstream cShaderStream;
        cShaderStream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        NT_INTER_CRITICAL("faild to load shader file: {}", shaderPath);
        return 0;
    }
    const char* shaderCode = computeCode.c_str();

    unsigned int shader;

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        NT_INTER_CRITICAL("shader: {} failed to compile with error message: {}", shader, infoLog);
        return 0;
    }

    return shader;
}

Shader Shader::LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath)
{
    unsigned int vertexShader = CompileShader(vertexPath, GL_VERTEX_SHADER);

    unsigned int fragmentShader = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        NT_INTER_CRITICAL("shader_program linking faild {}", infoLog);
        return Shader(0);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Renderer::AddShadow(shaderProgram);

    return Shader(shaderProgram);
}

Shader Shader::LoadAndCompileComputeShader(const char *computePath) {

    unsigned int shader = CompileShader(computePath, GL_COMPUTE_SHADER);

    unsigned shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        NT_INTER_CRITICAL("shader_program linking faild {}", infoLog);
        return Shader(0);
    }

    glDeleteShader(shader);

    return Shader(shaderProgram);
}

unsigned int Shader::bindTextureSlot(std::string bindingName)
{
    for (int i = 0; i < mBindings.size(); i++) {
        if (mBindings[i] == bindingName) {
            return i;
        }
    }
    glUseProgram(mShaderProgram);
    glUniform1i(glGetUniformLocation(mShaderProgram, bindingName.c_str()), mBindings.size() + 1);
    mBindings.push_back(bindingName);
    return mBindings.size();
}

void Shader::AddShadows()
{
    Renderer::AddShadow(mShaderProgram);
}

void Shader::UniformVec3(const char *name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(mShaderProgram, name), x, y, z);
}
void Shader::UniformVec3v(const char *name, const glm::vec3 &val)
{
    glUniform3fv(glGetUniformLocation(mShaderProgram, name), 1, &val[0]);
}
void Shader::UniformMat4v(const char *name, const glm::mat4 &val)
{
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, name), 1, GL_FALSE, &val[0][0]);
}
}  // namespace neith
