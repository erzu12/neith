#pragma once

#include "mesh.h"
#include "material.h"
#include "camera.h"
#include "window/window.h"

#include <cglm/cglm.h>

struct RenderContext {
    struct Materials *mat;
    struct Window *window;
    int *materials;
    int primitivesCount;
    mat4 *modelMats;
    mat4 lightSpaceMatrix;
    unsigned int *VAOs;
    int *indCounts;
    int **indices;
};

void DeleteStaticRender(struct RenderContext *rc) {
    free(rc->VAOs);
    free(rc);
};

struct Scene {
    struct StaticPrimitives *sp;
    struct Materials *mat;
    struct RenderContext *rc;
    struct CameraData *cd;

    vec3 lightDir;
};

struct Scene *InitScene(int maxPrimitives) {
    struct Scene *scene = malloc(sizeof(struct Scene));
    scene->sp = InitStaticPrimitives(maxPrimitives);
    scene->mat = InitMaterials(maxPrimitives);
    scene->cd = CameraInit();
    return scene;
}

void DeleteScene(struct Scene *sc) {
    DeleteMaterials(sc->mat);
    DeleteStaticRender(sc->rc);
    DeleteStaticPrimitives(sc->sp);
    free(sc->cd); 
}
