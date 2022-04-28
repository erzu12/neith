#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "scene/scene.h"
#include "window/window.h"

namespace neith {
class InstanceRenderer {
public:
    InstanceRenderer();

    void RenderInstanced(int width, int height);
    void RenderInstancedShadows(int shaderProgram);

private:
    unsigned int *VAOs;
    unsigned int **VBOs;
    glm::mat4 lightSpaceMatrix;
};
}  // namespace neith
