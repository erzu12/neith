#include "shaders.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>

#include "log.h"
#include "render/render.h"

namespace neith {
Shader Shader::LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath)
{
    const char *vertCode = LoadShader(vertexPath);
    const char *fragCode = LoadShader(fragmentPath);

    if (!vertCode || !fragCode) {
        return Shader(0);
    }

    const char *vertCodeArr[1] = { vertCode };
    const char *fragCodeArr[1] = { fragCode };

    int success;
    char infoLog[512];

    // VertexShader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, vertCodeArr, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        NT_INTER_CRITICAL("VertexShader: {} faild to compile:\n{}", vertexPath, infoLog);
        return Shader(0);
    }

    // Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, fragCodeArr, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        NT_INTER_CRITICAL("FragmentShader: {} faild to compile:\n{}", fragmentPath, infoLog);
        return Shader(0);
    }

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        NT_INTER_CRITICAL("shader_program linking faild {}", infoLog);
        return 0;
    }

    delete[] vertCode;
    delete[] fragCode;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Renderer::AddShadow(shaderProgram);

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
    glUniform1i(glGetUniformLocation(mShaderProgram, bindingName.c_str()), mBindings.size() - 1);
    mBindings.push_back(bindingName);
    return mBindings.size() - 1;
}

char *Shader::LoadShader(const char *path)
{
    std::ifstream ifs;

    ifs.open(path, std::ifstream::in);
    if (!ifs.is_open()) {
        NT_INTER_ERROR("faild to load: {}", path);
        return nullptr;
    }

    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    char *buffer = new char[length + 1];

    ifs.read(buffer, length);
    if (!ifs) {
        NT_INTER_ERROR("faild to read: {}", path);
        ifs.close();
        return nullptr;
    }
    ifs.close();

    buffer[length] = '\0';

    return buffer;
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
