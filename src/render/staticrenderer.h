#pragma once

#include <glad/glad.h>

#include "scene/scene.h"
#include "window/window.h"

void InitStaticRender(struct Scene *sc, struct Window *window);

void RenderStatic(struct Scene *sc, int width, int height);

void RenderStaticShadows(struct Scene *sc, int shaderProgram);
