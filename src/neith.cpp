#include "neith.h"

#include "window/window.h"
#include "scene/meshloader.h"
#include "render/render.h"
#include "scene/scene.h"
#include "scene/material.h"
#include "render/shaders.h"

struct Window* neith::nth_CreateWindow() {
    return CreateWindow();
}

struct Scene* neith::nth_InitScene(int maxPrimitives) {
    return InitScene(maxPrimitives);
}

void neith::nth_AttachSceneToWindow(struct Scene *sc, struct Window *win) {
    AttachSceneToWindow(sc, win);
}

void neith::nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount) {
    LoadModels(sc, paths, modelCount);
}

void neith::nth_InitRender(struct Scene *sc, struct Window *win) {
    InitRender(sc, win);
}

void neith::nth_UpdateWindow(struct Window *win) {
    UpdateWindow(win);
}

void neith::nth_UpdateRender(struct Scene *sc) {
    UpdateRender(sc);
}

unsigned int neith::nth_LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath) {
    return LoadAndCompileShaders(vertexPath, fragmentPath);
}

void neith::nth_SetShader(struct Materials *mat, int material, int shader) {
    SetShader(mat, material, shader);
}

int neith::nth_SetShaderByName(struct Materials *mat, char *materialName, int shader) {
    return SetShaderByName(mat, materialName, shader);
}

void neith::nth_SetTexture(struct Materials *mat, int material, int texture, char *bindingName) {
    SetTexture(mat, material, texture, bindingName);
}

int neith::nth_SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName) {
    return SetTextureByName(mat, materialName, texture, bindingName);
}


void neith::nth_SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value) {
    SetValueByNameF(mat, materialName, bindingName, value);
}

void neith::nth_SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value) {
    SetValueByNameV3(mat, materialName, bindingName, value);
}

void neith::nth_SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z) {
    SetValueByNameV3v(mat, materialName, bindingName, x, y, z);
}
