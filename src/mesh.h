#pragma once

#include "material.h"
#include "cglm/cglm.h"

#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

struct StaticPrimitives {
    int primitivesCount;
    int meshCount;
    mat4 *modelMats;
    int vertSize;
    int indSize;
    float *vertData;
    int *indData;
    int *meshes;
    int *vertOffsets;
    int *indOffsets;
    int *vertCounts;
    int *indCounts;
    int *materials;
};

int AddStaticPrimitive(struct StaticPrimitives *sp,
                       mat4 modelMat,
                       int vertOffset,
                       int vertCount,
                       int indOffset,
                       int indCount,
                       int material)
{
    int primitive = sp->primitivesCount;
    sp->primitivesCount = primitive + 1;

    glm_mat4_copy(modelMat, sp->modelMats[primitive]);
    sp->vertOffsets[primitive] = vertOffset;
    sp->indOffsets[primitive] = indOffset;
    sp->vertCounts[primitive] = vertCount;
    sp->indCounts[primitive] = indCount;
    sp->materials[primitive] = material;

    return primitive;
}

int AddStaticMesh(struct StaticPrimitives *primitives,
                  mat4 modelMat,
                  int vertOffsets[],
                  int vertCounts[],
                  int indOffsets[],
                  int indCounts[],
                  int materials[],
                  int primitivesCount)
{
    int mesh = primitives->meshCount;
    primitives->meshCount++;

    primitives->meshes[mesh * 2] = primitives->primitivesCount;
    primitives->meshes[mesh * 2 + 1] = primitives->primitivesCount + primitivesCount;

    for(int i = 0; i < primitivesCount; i++) {
        AddStaticPrimitive(primitives, modelMat, vertOffsets[i], vertCounts[i], indOffsets[i],
                           indCounts[i], materials[i]);
    }
    return mesh;
}

struct StaticPrimitives *InitStaticPrimitives(int maxPrimitives) {
    struct StaticPrimitives *sp = (struct StaticPrimitives*)malloc(sizeof(struct StaticPrimitives));
    sp->modelMats = malloc(sizeof(mat4) * maxPrimitives);
    sp->meshes = (int *)malloc(sizeof(int) * maxPrimitives * 2);
    sp->vertOffsets = (int *)malloc(sizeof(int) * maxPrimitives);
    sp->indOffsets = (int *)malloc(sizeof(int) * maxPrimitives);
    sp->vertCounts = (int *)malloc(sizeof(int) * maxPrimitives);
    sp->indCounts = (int *)malloc(sizeof(int) * maxPrimitives);
    sp->materials = (int *)malloc(sizeof(int) * maxPrimitives);

    sp->primitivesCount = 0;
    sp->meshCount = 0;
    sp->indSize = 0;
    sp->vertSize = 0;
    
    return sp;
}

void DeleteStaticPrimitives(struct StaticPrimitives *sp) {
    free(sp->modelMats);
    free(sp->meshes);
    free(sp->vertOffsets);
    free(sp->indOffsets);
    free(sp->vertCounts);
    free(sp->indCounts);
    free(sp->materials);
    free(sp);
}

//int AddDynamicMesh(struct DynamicMeshes *meshes, float *vertices, int vertSize, int *indices, int indSize) {
//    int mesh = meshes->meshCount + 1;
//    meshes->meshCount = mesh;
//    return mesh;
//};
