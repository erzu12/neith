#pragma once

#include "cglm/cglm.h"

struct StaticPrimitives {
    int primitivesCount;
    int meshCount;
    mat4 *modelMats;
    int vertSize;
    int indSize;
    float **vertices;
    int **indices;
    int *meshes;
    int *vertCounts;
    int *indCounts;
    int *materials;
};

int AddStaticPrimitive(struct StaticPrimitives *sp,
                       mat4 modelMat,
                       float *vertices,
                       int vertCount,
                       int *indices,
                       int indCount,
                       int material);

int AddStaticMesh(struct StaticPrimitives *sp, int primitivesCount);

struct StaticPrimitives *InitStaticPrimitives(int maxPrimitives);

void DeleteStaticPrimitives(struct StaticPrimitives *sp);
