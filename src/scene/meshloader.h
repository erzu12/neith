#pragma once

#include "scene.h"
#include <cgltf.h>
#include <glm/mat4x4.hpp>

namespace neith {
    void LoadModels(struct Scene *sc, const char* paths[], int modelCount);
    void PathToBinPath(const char *path, char* binPath, char* uri);
    void ReadTransform(cgltf_node *node, glm::mat4 &modelMat);
    bool CheckAtributeFormat(cgltf_primitive *primitive);
    bool HasTangents(cgltf_primitive *primitive);
    int ReadMaterial(struct Materials *mat, cgltf_material *material);
    char *LoadBinFile(int dataLength, char *binPath);
    float *LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount, bool hasTangents);
    int *LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount);
    void CalcTangents(float *vertices, int vertCount, int *indices, int indCount);

}
