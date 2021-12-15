#pragma once

#include <cglm/cglm.h>
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "render/render.h"

//struct RenderContext {
//   struct Materials *mat;
//   struct Window *win;
//   int *materials;
//   int primitivesCount;
//   mat4 *modelMats;
//   mat4 lightSpaceMatrix;
//   unsigned int *VAOs;
//   unsigned int texColorBuffer;
//   unsigned int screenTexture;
//   unsigned int screenShader;
//   unsigned int FBO;
//   unsigned int depthMapFBO;
//   unsigned int intermediateFBO;
//   unsigned int shadowMapShader;
//   unsigned int texColorBuffer;
//   unsigned int screenTexture;
//   unsigned int cubeMap;
//   unsigned int screenVAO;
//   unsigned int cubeMapVAO;
//   unsigned int cubeMapShader;
//   int *indCounts;
//   int **indices;
//};

class Scene {
public:
    StaticPrimitives *sp;
    Materials *mat;
    //struct RenderContext *rc;
    Camera *cd;
    Renderer *re;
    vec3 lightDir;

    Scene(int maxPrimitives);

    void InitRenderer(struct Scene *sc, struct Window *window);
    void UpdateRender(struct Scene *sc);

    ~Scene();
};

void DeleteStaticRender(struct RenderContext *rc);
