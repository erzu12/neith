#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "scene/scene.h"
#include "window/window.h"

namespace neith {
class StaticRenderer {
public:
    unsigned int *VAOs;
    glm::mat4 lightSpaceMatrix;

    StaticRenderer(struct Scene *sc, struct Window *window);

    void RenderStatic(struct Scene *sc, int width, int height);

    void RenderStaticShadows(struct Scene *sc, int shaderProgram);
};
}  // namespace neith
