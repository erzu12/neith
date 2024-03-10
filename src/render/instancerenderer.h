#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "render/shaders.h"
#include "window/window.h"

namespace neith {
class InstanceRenderer {
public:
    InstanceRenderer();

    void renderSystem(int width, int height, unsigned int depthMap);

    //void RenderInstanced(int width, int height, unsigned int depthMap);
    void RenderInstancedShadows(Shader &shaderProgram, unsigned int depthMap);

private:
    glm::mat4 mLightSpaceMatrix;
};
}  // namespace neith
