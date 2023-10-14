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
#include "physics/physics.h"
#include "render/render.h"
#include "scene/model.h"
#include "scene/scene.h"
#include "timer.h"
#include "window/input.h"
#include "window/window.h"
#include "scene/components/cameraComp.h"
#include "log.h"

namespace neith {
void Init();

void InitRenderer();

Model *LoadModel(std::string path);

void CalcNormals(float *vertices, int vertCount, int *indices, int indCount);
void CalcTangents(float *vertices, int vertCount, int *indices, int indCount);


//void AttachCamera(unsigned int entityID);

bool ShouldClose();

void Update();

void UpdateWindow();

void UpdateRender();

Shader nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath);


//void MakeBackfaced(Model *model, int material);
//void MakeBackfaced(unsigned int material);

unsigned int LoadTexture(const char *path, GLint colorSpace, GLint internalColorSpace);

bool GetKey(Key key);
glm::vec2 GetDeltaMouse();

//int AddRigidBody(unsigned int entityID, Collider *collider, float mass);

//void UpdatePhysics();

//int GetContacPoints(unsigned int colliderID, ContactPoint *contactPoints, int capacity);

double OpenSimplex2D(double x, double y, int octaves = 1, float persistence = 0.5f, float lacunarity = 2);
double Voronoi2D(double x, double y, float randomness = 1.0f);
double VoronoiDistace2D(double x, double y, float randomness = 1.0f);
}  // namespace neith
