#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

namespace neith {
class Shader {
public:
    unsigned int mShaderProgram;
    Shader() : mShaderProgram(0) {}
    Shader(unsigned int shaderProgram) : mShaderProgram(shaderProgram) {}
    static Shader LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);
    static Shader LoadAndCompileComputeShader(const char* computePath);
    void UniformVec3(const char* name, float x, float y, float z);
    void UniformVec3v(const char* name, const glm::vec3& val);
    void UniformMat4v(const char* name, const glm::mat4& val);
    unsigned int bindTextureSlot(std::string bindingName);
private:
    std::vector<std::string> mBindings;
    static char* LoadShader(const char* path);
};
}  // namespace neith
