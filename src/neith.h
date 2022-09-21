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
// remove
#include "scene/components/meshComp.h"
#include "scene/model.h"
#include "scene/scene.h"
#include "timer.h"
#include "window/input.h"
#include "window/window.h"

namespace neith {
void Init();

void InitRenderer();

Model *LoadModel(std::string path);

unsigned int AddEntity(std::string name);
unsigned int AddEntity(std::string name, glm::mat4 transform);

unsigned int AddMesh(int primitivesCount);

void CalcNormals(float *vertices, int vertCount, int *indices, int indCount);
void CalcTangents(float *vertices, int vertCount, int *indices, int indCount);

unsigned int AddPrimitive(
    float *vertices, int vertCount, int *indices, int indCount, unsigned int mesh, int LOD, unsigned int material);

void SetLODs(unsigned int meshID, int LODCount, float *LODDistances);

void AddMeshToEntity(unsigned int entityID, unsigned int meshID);

void AddModelToEntity(unsigned int entityID, Model *model);

void AttachCamera(unsigned int entityID);

bool ShouldClose();

void Update();

void UpdateWindow();

void UpdateRender();

unsigned int nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath);

unsigned int AddMaterial();

void SetShader(unsigned int meshID, int material, int shader);
void SetShader(Model *model, int material, int shader);
void SetShader(unsigned int material, int shader);

void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName);
void SetTexture(Model *model, int material, int texture, const char *bindingName);

void SetTransparancyTexture(unsigned int meshID, int material, int texture, const char *bindingName = "material.alpha");
void SetTransparancyTexture(Model *model, int material, int texture, const char *bindingName = "material.alpha");

void SetValue(unsigned int meshID, int material, const char *bindingName, float value);
void SetValue(Model *model, int material, const char *bindingName, float value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float *value);
void SetValue(Model *model, int material, const char *bindingName, float *value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z);
void SetValue(Model *model, int material, const char *bindingName, float x, float y, float z);

void MakeBackfaced(Model *model, int material);
void MakeBackfaced(unsigned int material);

unsigned int LoadTexture(const char *path, GLint colorSpace, GLint internalColorSpace);

void TransformEntity(unsigned int entityID, glm::mat4 &transform);

bool GetKey(Key key);
glm::vec2 GetDeltaMouse();

int AddRigidBody(unsigned int entityID, Collider *collider, float mass);

void UpdatePhysics();

int GetContacPoints(unsigned int colliderID, ContactPoint *contactPoints, int capacity);

double OpenSimplex2D(double x, double y, int octaves = 1, float persistence = 0.5f, float lacunarity = 2);
double Voronoi2D(double x, double y, float randomness = 1.0f);
double VoronoiDistace2D(double x, double y, float randomness = 1.0f);
}  // namespace neith
