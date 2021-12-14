#pragma once

#include <cglm/cglm.h>

struct RenderContext {
    struct Materials *mat;
    struct Window *win;
    int *materials;
    int primitivesCount;
    mat4 *modelMats;
    mat4 lightSpaceMatrix;
    unsigned int *VAOs;
    unsigned int FBO;
    unsigned int texColorBuffer;
    unsigned int intermediateFBO;
    unsigned int screenTexture;
    unsigned int screenVAO;
    unsigned int screenShader;
    unsigned int depthMapFBO;
    unsigned int shadowMapShader;
    unsigned int cubeMap;
    unsigned int cubeMapVAO;
    unsigned int cubeMapShader;
    int *indCounts;
    int **indices;
};

struct Scene {
    struct StaticPrimitives *sp;
    struct Materials *mat;
    struct RenderContext *rc;
    struct CameraData *cd;

    vec3 lightDir;
};

void DeleteStaticRender(struct RenderContext *rc);

struct Scene;

struct Scene *InitScene(int maxPrimitives);

void DeleteScene(struct Scene *sc);
