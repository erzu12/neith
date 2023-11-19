#include "neith.h"

#include "physics/colliders.h"
#include "procedural/openSimplex.h"
#include "procedural/voronoi.h"
#include "render/shaders.h"
#include "render/textures.h"
#include "scene/meshloader.h"
#include "window/window.h"
#include "scene/mesh.h"
#include "scene/ecsmanager.h"
#include "scene/material.h"

namespace neith {
void Init()
{
    new Window();
    new Physics();
    new Renderer();
}

Model *LoadModel(std::string path) { return ModelLoader::LoadModel(path); }

void CalcNormals(float *vertices, int vertCount, int *indices, int indCount)
{
    ModelLoader::CalcNormals(vertices, vertCount, indices, indCount);
}
void CalcTangents(float *vertices, int vertCount, int *indices, int indCount)
{
    ModelLoader::CalcTangents(vertices, vertCount, indices, indCount);
}

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

Shader nth_LoadAndCompileShaders(const char *vertexPath, const char *fragmentPath)
{
    return Shader::LoadAndCompileShaders(vertexPath, fragmentPath);
}

//void MakeBackfaced(Model *model, int material) { system::MakeBackfaced(model, material); }
//void MakeBackfaced(unsigned int material) { Materials::MakeBackfaced(material); }

unsigned int LoadTexture(const char *path, GLint colorSpace, GLint internalColorSpace)
{
    return texture::LoadTexture(path, colorSpace, internalColorSpace);
}

bool GetKey(Key key) { return Input::GetKeyDown(key); }

glm::vec2 GetDeltaMouse() { return Input::GetDeltaMouse(); }

//int AddRigidBody(unsigned int entityID, Collider *collider, float mass)
//{
    //return system::AddRigidBody(entityID, collider, mass);
//}

//void UpdatePhysics() { Physics::UpdatePhysics(); }

//int GetContacPoints(unsigned int colliderID, ContactPoint *contactPoints, int capacity)
//{
    //return Physics::GetContacPoints(colliderID, contactPoints, capacity);
//}
double OpenSimplex2D(double x, double y, int octaves, float persistence, float lacunarity)
{
    return OpenSimplex::Noise2D(x, y, octaves, persistence, lacunarity);
}
double Voronoi2D(double x, double y, float randomness) { return Voronoi::Voronoi2D(x, y, randomness); }
double VoronoiDistace2D(double x, double y, float randomness) { return Voronoi::VoronoiDistace2D(x, y, randomness); }
}  // namespace neith
