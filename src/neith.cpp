#include "neith.h"

#include "physics/colliders.h"
#include "render/shaders.h"
#include "render/textures.h"
#include "scene/material.h"
#include "scene/meshloader.h"
#include "scene/scene.h"
#include "scene/systems/sysCamera.h"
#include "scene/systems/sysMaterial.h"
#include "scene/systems/sysMesh.h"
#include "scene/systems/sysPhysics.h"
#include "scene/systems/sysTransform.h"
#include "window/window.h"

namespace neith {
void Init()
{
    new Window();
    new Scene();
    new Physics();
}

// Window *nth_CreateWindow() {}

void InitRenderer() { new Renderer(); }

Model *LoadModel(std::string path) { return ModelLoader::LoadModel(path); }

unsigned int AddEntity(std::string name)
{
    glm::mat4 transform = glm::mat4(1.0f);
    return Entity::AddEntity(0, transform, name);
}
unsigned int AddEntity(std::string name, glm::mat4 transform) { return Entity::AddEntity(0, transform, name); }

void AddMeshToEntity(unsigned int entityID, unsigned int meshID) { system::AddMeshToEntity(entityID, meshID); }

void AddModelToEntity(unsigned int entityID, Model *model) { system::AddModelToEntity(entityID, model); }

void AttachCamera(unsigned int entityID) { system::AddCamera(entityID); }

// Renderer *nth_InitRender(Scene *sc, Window *win) { return new Renderer(sc, win); }

bool ShouldClose() { return Window::ShouldClose(); }

void Update()
{
    Window::UpdateWindow();
    Renderer::UpdateRender();
    Physics::UpdatePhysics();
}

void UpdateWindow(Window *win) { Window::UpdateWindow(); }

void UpdateRender() { Renderer::UpdateRender(); }

unsigned int nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath)
{
    return LoadAndCompileShaders(vertexPath, fragmentPath);
}

void SetShader(unsigned int meshID, int material, int shader) { system::SetShader(meshID, material, shader); }
void SetShader(Model *model, int material, int shader) { system::SetShader(model, material, shader); }

void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName)
{
    system::SetTexture(meshID, material, texture, bindingName);
}

void SetTexture(Model *model, int material, int texture, const char *bindingName)
{
    system::SetTexture(model, material, texture, bindingName);
}

void SetTransparancyTexture(unsigned int meshID, int material, int texture, const char *bindingName)
{
    system::SetTransparancyTexture(meshID, material, texture, bindingName);
}
void SetTransparancyTexture(Model *model, int material, int texture, const char *bindingName)
{
    system::SetTransparancyTexture(model, material, texture, bindingName);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float value)
{
    system::SetValue(meshID, material, bindingName, value);
}

void SetValue(Model *model, int material, const char *bindingName, float value)
{
    system::SetValue(model, material, bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float *value)
{
    system::SetValue(meshID, material, bindingName, value);
}

void SetValue(Model *model, int material, const char *bindingName, float *value)
{
    system::SetValue(model, material, bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z)
{
    system::SetValue(meshID, material, bindingName, x, y, z);
}

void SetValue(Model *model, int material, const char *bindingName, float x, float y, float z)
{
    system::SetValue(model, material, bindingName, x, y, z);
}

// int nth_SetShaderByName(struct Materials *mat, char *materialName, int shader) {
// return mat->SetShaderByName(materialName, shader);
//}

unsigned int LoadTexture(const char *path, GLint colorSpace, GLint internalColorSpace)
{
    return texture::LoadTexture(path, colorSpace, internalColorSpace);
}

// int nth_SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName) {
// return mat->SetTextureByName(materialName, texture, bindingName);
//}

// void nth_SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value) {
// mat->SetValueByNameF(materialName, bindingName, value);
//}

// void nth_SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value) {
// mat->SetValueByNameV3(materialName, bindingName, value);
//}

// void nth_SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z) {
// mat->SetValueByNameV3v(materialName, bindingName, x, y, z);
//}

void TransformEntity(unsigned int entityID, glm::mat4 &transform) { system::Transform(entityID, transform); }

bool GetKey(Key key) { return Input::GetKeyDown(key); }

glm::vec2 GetDeltaMouse() { return Input::GetDeltaMouse(); }

int AddRigidBody(unsigned int entityID, Collider *collider, float mass)
{
    return system::AddRigidBody(entityID, collider, mass);
}

void UpdatePhysics() { Physics::UpdatePhysics(); }

int GetContacPoints(unsigned int colliderID, ContactPoint *contactPoints, int capacity)
{
    return Physics::GetContacPoints(colliderID, contactPoints, capacity);
}

}  // namespace neith
