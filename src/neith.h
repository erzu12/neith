/*
 * Include this file in your Project.
 */

#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include "log.h"
#include "render/render.h"
#include "scene/scene.h"
#include "timer.h"
#include "window/input.h"
#include "window/window.h"

namespace neith {
Window *nth_CreateWindow();

Scene *nth_InitScene(int maxPrimitives);

// void nth_LoadModels(Scene *sc, const char *paths[], int modelCount);

// int LoadModel(std::string path);

int LoadModel(std::string path, int &outMeshCount);

unsigned int AddEntity(std::string name);

void AddMeshToEntity(unsigned int entityID, unsigned int meshID, glm::mat4 modelMat);

void AttachCamera(unsigned int entityID);

Renderer *nth_InitRender(Scene *sc, Window *win);

void nth_UpdateWindow(Window *win);

void nth_UpdateRender(Renderer *re, Scene *sc);

unsigned int nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath);

void SetShader(unsigned int meshID, int material, int shader);

// int nth_SetShaderByName(Materials *mat, char *materialName, int shader);

void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName);

void SetValue(unsigned int meshID, int material, const char *bindingName, float value);

void SetValue(unsigned int meshID, int material, const char *bindingName, float *value);

void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z);

// int nth_SetTextureByName(Materials *mat, char *materialName, int texture, char *bindingName);

// void nth_SetValueByNameF(Materials *mat, char *materialName, char *bindingName, float value);

// void nth_SetValueByNameV3(Materials *mat, char *materialName, char *bindingName, float *value);

// void nth_SetValueByNameV3v(Materials *mat, char *materialName, char *bindingName, float x, float y, float z);

void TransformEntity(unsigned int entityID, glm::mat4 &transform);

bool GetKey(Window *window, Key key);
glm::vec2 GetDeltaMouse(Window *window);
}  // namespace neith
