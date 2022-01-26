/*
 * Include this file in your Project.
 */

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene/scene.h"
#include "window/window.h"
#include "render/render.h"

namespace neith {
    struct Window* nth_CreateWindow();

    struct Scene* nth_InitScene(int maxPrimitives);

    void nth_AttachSceneToWindow(struct Scene *sc, struct Window *win);

    void nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount);

    Renderer *nth_InitRender(struct Scene *sc, struct Window *win);

    void nth_UpdateWindow(struct Window *win);

    void nth_UpdateRender(Renderer *re, struct Scene *sc);

    unsigned int nth_LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);

    void nth_SetShader(Materials *mat, int material, int shader);

    int nth_SetShaderByName(Materials *mat, char *materialName, int shader);

    void nth_SetTexture(Materials *mat, int material, int texture, char *bindingName);

    int nth_SetTextureByName(Materials *mat, char *materialName, int texture, char *bindingName);

    void nth_SetValueByNameF(Materials *mat, char *materialName, char *bindingName, float value);

    void nth_SetValueByNameV3(Materials *mat, char *materialName, char *bindingName, float *value);

    void nth_SetValueByNameV3v(Materials *mat, char *materialName, char *bindingName, float x, float y, float z);
}
