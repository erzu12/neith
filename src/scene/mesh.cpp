#include "mesh.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "material.h"

namespace neith {
StaticPrimitives::StaticPrimitives(int maxPrimitives)
{
    modelMats = (glm::mat4 *)malloc(sizeof(glm::mat4) * maxPrimitives);
    meshes = (int *)malloc(sizeof(int) * maxPrimitives * 3);
    vertices = (float **)calloc(sizeof(float *), maxPrimitives);
    indices = (int **)calloc(sizeof(int *), maxPrimitives);
    vertCounts = (int *)malloc(sizeof(int) * maxPrimitives);
    indCounts = (int *)malloc(sizeof(int) * maxPrimitives);
    materials = (int *)malloc(sizeof(int) * maxPrimitives);

    primitivesCount = 0;
    meshCount = 0;
}

int StaticPrimitives::AddStaticPrimitive(
    glm::mat4 modelMat, float *vertices, int vertCount, int *indices, int indCount, int material)
{
    int primitive = primitivesCount;
    primitivesCount++;

    modelMats[primitive] = modelMat;
    this->vertices[primitive] = vertices;
    this->indices[primitive] = indices;
    this->vertCounts[primitive] = vertCount;
    this->indCounts[primitive] = indCount;
    this->materials[primitive] = material;

    return primitive;
}

int StaticPrimitives::AddStaticMesh(int primitivesCount)
{
    int mesh = meshCount;
    meshCount++;

    int primitive = primitivesCount;
    primitivesCount = primitive + primitivesCount;

    meshes[mesh * 3] = primitivesCount;
    meshes[mesh * 3 + 1] = 0;
    meshes[mesh * 3 + 2] = primitivesCount + primitivesCount;

    return mesh;
}

StaticPrimitives::~StaticPrimitives()
{
    free(modelMats);
    free(meshes);
    free(vertices);
    free(indices);
    free(vertCounts);
    free(indCounts);
    free(materials);
}

// int AddDynamicMesh(struct DynamicMeshes *meshes, float *vertices, int vertSize, int *indices, int indSize) {
//     int mesh = meshes->meshCount + 1;
//     meshes->meshCount = mesh;
//     return mesh;
// };
}  // namespace neith
