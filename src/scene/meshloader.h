#pragma once

#include <cgltf.h>

#include <glm/mat4x4.hpp>

#include "scene.h"
#include "scene/model.h"

namespace neith {
class ModelLoader {
public:
    // static void LoadModels(const char* paths[], int modelCount);
    static Model *LoadModel(std::string path);
    static void CalcTangents(float *vertices, int vertCount, int *indices, int indCount);
    static void CalcNormals(float *vertices, int vertCount, int *indices, int indCount);

private:
    static void PathToBinPath(const char *path, char *binPath, char *uri);
    static void ReadTransform(cgltf_node *node, glm::mat4 &modelMat);
    static bool CheckAtributeFormat(cgltf_primitive *primitive);
    static bool HasTangents(cgltf_primitive *primitive);
    static Material *ReadMaterial(cgltf_material **gltfMaterials, cgltf_material *gltfMaterial);
    // static int ReadMaterial(cgltf_material **gltfMaterials,
    // int &materialsCount,
    // cgltf_material *material,
    // int *materials);
    static char *LoadBinFile(int dataLength, char *binPath);
    static float *LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount, bool hasTangents);
    static int *LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount);
};

}  // namespace neith
