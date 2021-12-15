#pragma once

#include "cglm/cglm.h"

class StaticPrimitives {
public:
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

    StaticPrimitives(int maxPrimitives);

    int AddStaticPrimitive(mat4 modelMat,
                           float *vertices,
                           int vertCount,
                           int *indices,
                           int indCount,
                           int material);

    int AddStaticMesh(int primitivesCount);

    ~StaticPrimitives();
};
