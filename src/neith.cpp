#include "neith.h"

#include "render/shaders.h"
#include "scene/material.h"
#include "scene/meshloader.h"
#include "scene/scene.h"
#include "scene/systems/sysCamera.h"
#include "scene/systems/sysMaterial.h"
#include "scene/systems/sysMesh.h"
#include "scene/systems/sysTransform.h"
#include "window/window.h"

namespace neith {
Window *nth_CreateWindow() { return new Window(); }

void InitScene() {
    new Scene();
    new Renderer();
}

//Scene *nth_InitScene() { return new Scene(); }

// void nth_LoadModels(Scene *sc, const char *paths[], int modelCount)
//{
//// LoadModels(sc, paths, modelCount);
//}

int LoadModel(std::string path, int &outMeshCount) { return ModelLoader::LoadModel(path, outMeshCount); }

unsigned int AddEntity(std::string name) { return Scene::AddEntity(name); }

void AddMeshToEntity(unsigned int entityID, unsigned int meshID, glm::mat4 modelMat)
{
    system::AddMeshToEntity(entityID, meshID, modelMat);
}

void AttachCamera(unsigned int entityID) { system::AddCamera(entityID); }

//Renderer *nth_InitRender(Scene *sc, Window *win) { return new Renderer(sc, win); }

void Update() {
    Window::UpdateWindow();
    Renderer::UpdateRender();
}

void UpdateWindow(Window *win) { Window::UpdateWindow(); }

void UpdateRender() { Renderer::UpdateRender(); }

unsigned int nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath)
{
    return LoadAndCompileShaders(vertexPath, fragmentPath);
}

void SetShader(unsigned int meshID, int material, int shader) { system::SetShader(meshID, material, shader); }

// int nth_SetShaderByName(struct Materials *mat, char *materialName, int shader) {
// return mat->SetShaderByName(materialName, shader);
//}

void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName)
{
    system::SetTexture(meshID, material, texture, bindingName);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float value)
{
    system::SetValue(meshID, material, bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float *value)
{
    system::SetValue(meshID, material, bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z)
{
    system::SetValue(meshID, material, bindingName, x, y, z);
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
}  // namespace neith
