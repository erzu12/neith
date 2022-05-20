/*
 * Include this file in your Project.
 */

#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include "debug.h"
#include "log.h"
#include "physics/physics.h"
#include "render/render.h"
#include "scene/scene.h"
#include "timer.h"
#include "window/input.h"
#include "window/window.h"

namespace neith {
Window *nth_CreateWindow();

void InitScene();

int LoadModel(std::string path, int &outMeshCount);

unsigned int AddEntity(std::string name);
unsigned int AddEntity(std::string name, glm::mat4 transform);

void AddMeshToEntity(unsigned int entityID, unsigned int meshID);

void AttachCamera(unsigned int entityID);

void Update();

void UpdateWindow();

void UpdateRender();

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

bool GetKey(Key key);
glm::vec2 GetDeltaMouse();

int AddRigidBody(unsigned int entityID, Collider *collider, float mass);

void UpdatePhysics();

int GetContacPoints(unsigned int colliderID, ContactPoint *contactPoints, int capacity);
}  // namespace neith
