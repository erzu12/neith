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
                       int material)
{
    //int primitive = sp->meshes[mesh * 3] + sp->meshes[mesh * 3 + 1];
    //if(primitive >= sp->meshes[mesh * 3 + 2]) 
    //    return 0;
    int primitive = sp->primitivesCount;
    sp->primitivesCount++;

    glm_mat4_copy(modelMat, sp->modelMats[primitive]);
    sp->vertices[primitive] = vertices;
    sp->indices[primitive] = indices;
    sp->vertCounts[primitive] = vertCount;
    sp->indCounts[primitive] = indCount;
    sp->materials[primitive] = material;

    return primitive;
}

int AddStaticMesh(struct StaticPrimitives *sp, int primitivesCount) {
    int mesh = sp->meshCount;
    sp->meshCount++;

    int primitive = sp->primitivesCount;
    sp->primitivesCount = primitive + primitivesCount;

    sp->meshes[mesh * 3] = sp->primitivesCount;
    sp->meshes[mesh * 3 + 1] = 0;
    sp->meshes[mesh * 3 + 2] = sp->primitivesCount + primitivesCount;

    return mesh;
}

struct StaticPrimitives *InitStaticPrimitives(int maxPrimitives) {
    struct StaticPrimitives *sp = (struct StaticPrimitives*)malloc(sizeof(struct StaticPrimitives));
    sp->modelMats = malloc(sizeof(mat4) * maxPrimitives);
    sp->meshes = malloc(sizeof(int) * maxPrimitives * 3);
    sp->vertices = calloc(sizeof(float *), maxPrimitives);
    sp->indices= calloc(sizeof(int *), maxPrimitives);
    sp->vertCounts = malloc(sizeof(int) * maxPrimitives);
    sp->indCounts = malloc(sizeof(int) * maxPrimitives);
    sp->materials = malloc(sizeof(int) * maxPrimitives);

    sp->primitivesCount = 0;
    sp->meshCount = 0;
    sp->indSize = 0;
    sp->vertSize = 0;
    
    return sp;
}

void DeleteStaticPrimitives(struct StaticPrimitives *sp) {
    free(sp->modelMats);
    free(sp->meshes);
    free(sp->vertices);
    free(sp->indices);
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
