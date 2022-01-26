#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "scene/scene.h"
#include "window/window.h"

namespace neith {
    class InstanceRenderer {
    public:
        unsigned int *VAOs;
        unsigned int **VBOs;
        glm::mat4 lightSpaceMatrix;

        InstanceRenderer(struct Scene *sc, struct Window *window);

        void RenderInstanced(struct Scene *sc, int width, int height);

        void RenderInstancedShadows(struct Scene *sc, int shaderProgram);
    };
}
