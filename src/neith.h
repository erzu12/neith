/*
 * Include this file in your Project.
 */

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene/scene.h"
#include "window/window.h"

namespace neith {
    struct Window* nth_CreateWindow();

    struct Scene* nth_InitScene(int maxPrimitives);

    void nth_AttachSceneToWindow(struct Scene *sc, struct Window *win);

    void nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount);

    void nth_InitRender(struct Scene *sc, struct Window *win);

    void nth_UpdateWindow(struct Window *win);

    void nth_UpdateRender(struct Scene *sc);

    unsigned int nth_LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath);

    void nth_SetShader(struct Materials *mat, int material, int shader);

    int nth_SetShaderByName(struct Materials *mat, char *materialName, int shader);

    void nth_SetTexture(struct Materials *mat, int material, int texture, char *bindingName);

    int nth_SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName);

    void nth_SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value);

    void nth_SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value);

    void nth_SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z);
}
