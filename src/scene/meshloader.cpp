#include "meshloader.h"

#include <cgltf.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>

#include "dataStructures.h"
#include "log.h"
#include "material.h"
#include "mesh.h"
#include "scene.h"
#include "systems/sysMesh.h"

namespace neith {
Model *ModelLoader::LoadModel(std::string path)
{
    // StaticPrimitives *sp = sc->sp;

    // for(int i = 0; i < modelCount; i++) {
    cgltf_options options = { (cgltf_file_type)0 };
    cgltf_data *gltfData = NULL;
    // cgltf_result result = cgltf_parse_file(&options, paths[i], &gltfData);
    cgltf_result result = cgltf_parse_file(&options, path.data(), &gltfData);
    if (result != cgltf_result_success) {
        // printf("faild to load model: %s\n", paths[i]);
        NT_INTER_ERROR("faild to load model: {}", path.data());
        return 0;
    }

    char binPath[1024];
    // PathToBinPath(paths[i], binPath, gltfData->buffers->uri);
    PathToBinPath(path.data(), binPath, gltfData->buffers->uri);

    int dataSize = gltfData->buffers->size;
    char *sceneData = LoadBinFile(dataSize, binPath);
    if (!sceneData) {
        return 0;
    }

    int nodeCount = gltfData->nodes_count;

    cgltf_material *gltfMaterials[gltfData->materials_count];
    int materials[gltfData->materials_count];
    int materialsCount = 0;

    int meshesLength = gltfData->meshes_count;
    std::unordered_map<cgltf_mesh *, unsigned int> meshes;

    Model *model = new Model();

    for (int j = 0; j < meshesLength; j++) {
        int primitivesCount = gltfData->nodes[j].mesh->primitives_count;

        unsigned int mesh = system::AddMesh(primitivesCount);

        meshes.insert({ &gltfData->meshes[j], mesh });

        for (int k = 0; k < primitivesCount; k++) {
            if (!CheckAtributeFormat(&gltfData->meshes[j].primitives[k])) {
                NT_INTER_WARN("WARNING: Vertex Attribute missing on mesh {}", gltfData->meshes[j].name);
                continue;
            }
            bool hasTangents = HasTangents(&gltfData->meshes[j].primitives[k]);

            int vertCount = gltfData->meshes[j].primitives[k].attributes[0].data->count;
            int indCount = gltfData->meshes[j].primitives[k].indices->count;

            float *vertices = LoadVertices(sceneData, gltfData->meshes[j].primitives[k], vertCount, hasTangents);
            int *indices = LoadIndices(sceneData, gltfData->meshes[j].primitives[k], indCount);
            if (!hasTangents)
                CalcTangents(vertices, vertCount, indices, indCount);

            int material = ReadMaterial(gltfMaterials, gltfData->meshes[j].primitives[k].material, model);

            system::AddStaticPrimitive(vertices, vertCount, indices, indCount, material);
        }
    }

    for (int j = 0; j < nodeCount; j++) {
        if (gltfData->nodes[j].mesh == NULL)
            continue;

        unsigned int mesh = meshes.at(gltfData->nodes[j].mesh);

        glm::mat4 modelMat(1.0f);
        ReadTransform(&gltfData->nodes[j], modelMat);

        model->AddInstance(mesh, modelMat);
    }
    //}
    return model;
}

void ModelLoader::PathToBinPath(const char *path, char *binPath, char *uri)
{
    char *lastSlash = (char *)strrchr(path, '/');
    int dirPathLen = lastSlash - path + 1;

    strncpy(binPath, path, dirPathLen);
    strncpy(binPath + dirPathLen, uri, 1024 - dirPathLen);
}

void ModelLoader::ReadTransform(cgltf_node *node, glm::mat4 &modelMat)
{
    if (node->has_translation) {
        glm::vec3 glmVec = glm::make_vec3(node->translation);
        modelMat = glm::translate(modelMat, glmVec);
    }
    if (node->has_rotation) {
        glm::quat rotation = glm::quat(node->rotation[3], node->rotation[0], node->rotation[1], node->rotation[2]);
        modelMat = modelMat * glm::toMat4(rotation);
    }
    if (node->has_scale) {
        modelMat = glm::scale(modelMat, glm::make_vec3(node->scale));
    }
}

bool ModelLoader::CheckAtributeFormat(cgltf_primitive *primitive)
{
    // refactor this garbage
    if (primitive->attributes_count < 3)
        return false;

    if (strcmp(primitive->attributes[0].name, "POSITION") != 0)
        return false;
    if (strcmp(primitive->attributes[1].name, "NORMAL") != 0)
        return false;
    if (strcmp(primitive->attributes[2].name, "TEXCOORD_0") != 0) {
        if (primitive->attributes_count < 4)
            return false;
        if (strcmp(primitive->attributes[3].name, "TEXCOORD_0") != 0)
            return false;
    }
    return true;
}

bool ModelLoader::HasTangents(cgltf_primitive *primitive)
{
    if (strcmp(primitive->attributes[2].name, "TANGENT") != 0)
        return false;
    return true;
}

int ModelLoader::ReadMaterial(cgltf_material **gltfMaterials, cgltf_material *gltfMaterial, Model *model)
{
    if (gltfMaterial == NULL)
        return 0;

    std::vector<unsigned int> *materials = model->GetMaterials();
    for (int i = 0; i < materials->size(); i++) {
        if (gltfMaterial == gltfMaterials[i]) {
            return materials->at(i);
        }
    }
    unsigned int newMat = Materials::AddMaterial();
    gltfMaterials[materials->size()] = gltfMaterial;
    materials->push_back(newMat);

    // NT_INTER_INFO(newMat);
    return newMat;
}

char *ModelLoader::LoadBinFile(int dataLength, char *binPath)
{
    // FILE * pFile;

    std::ifstream ifs;

    ifs.open(binPath, std::ifstream::in | std::ifstream::binary);

    // pFile = fopen(binPath, "rb");

    if (!ifs.is_open()) {
        NT_INTER_ERROR("faild to Load binary file: {}", binPath);
        return nullptr;
    }

    // if(!pFile) {
    // printf("faild to Load binary file: %s\n", binPath);
    // exit(EXIT_FAILURE);
    //}

    char *sceneData = new char[dataLength];
    // char *sceneData = (char*)malloc(dataLength);
    // if(sceneData == NULL) {
    // printf("memory error: unable to allocate sceneData");
    // exit(EXIT_FAILURE);
    //}

    // int readResult = fread(sceneData, 1, dataLength, pFile);
    // if(readResult != dataLength) {
    // printf("error reading file: %s\n", binPath);
    // exit(EXIT_FAILURE);
    //}

    ifs.read(sceneData, dataLength);
    if (!ifs) {
        NT_INTER_ERROR("faild to read: {}", binPath);
        ifs.close();
        return nullptr;
    }

    ifs.close();
    // fclose(pFile);

    return sceneData;
}

float *ModelLoader::LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount, bool hasTangents)
{
    float *vertData = (float *)sceneData;
    float *vertices = (float *)malloc(sizeof(float) * 12 * vertCount);

    int posOffset = primitive.attributes[0].data->buffer_view->offset;
    int normalOffset = primitive.attributes[1].data->buffer_view->offset;
    int tangentOffset, texCoordOffset;
    if (hasTangents) {
        tangentOffset = primitive.attributes[2].data->buffer_view->offset;
        texCoordOffset = primitive.attributes[3].data->buffer_view->offset;
    }
    else {
        texCoordOffset = primitive.attributes[2].data->buffer_view->offset;
    }

    for (int i = 0; i < vertCount; i++) {
        vertices[i * 12] = vertData[posOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 1] = vertData[posOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 2] = vertData[posOffset / sizeof(float) + i * 3 + 2];

        vertices[i * 12 + 3] = vertData[normalOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 4] = vertData[normalOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 5] = vertData[normalOffset / sizeof(float) + i * 3 + 2];

        if (hasTangents) {
            vertices[i * 12 + 6] = vertData[tangentOffset / sizeof(float) + i * 4];
            vertices[i * 12 + 7] = vertData[tangentOffset / sizeof(float) + i * 4 + 1];
            vertices[i * 12 + 8] = vertData[tangentOffset / sizeof(float) + i * 4 + 2];
            vertices[i * 12 + 9] = vertData[tangentOffset / sizeof(float) + i * 4 + 3];
        }

        vertices[i * 12 + 10] = vertData[texCoordOffset / sizeof(float) + i * 2];
        vertices[i * 12 + 11] = vertData[texCoordOffset / sizeof(float) + i * 2 + 1];
    }
    return vertices;
}

int *ModelLoader::LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount)
{
    int *indices = (int *)malloc(sizeof(int) * indCount);

    int indOffset = primitive.indices->buffer_view->offset;

    if (primitive.indices->component_type == cgltf_component_type_r_16u) {
        unsigned short *indData = (unsigned short *)&sceneData[indOffset];
        for (int i = 0; i < indCount; i++) {
            indices[i] = indData[i];
        }
    }
    else {
        unsigned int *indData = (unsigned int *)&sceneData[indOffset];
        for (int i = 0; i < indCount; i++) {
            indices[i] = indData[i];
        }
    }
    return indices;
}

void ModelLoader::CalcTangents(float *vertices, int vertCount, int *indices, int indCount)
{
    glm::vec3 *tan1 = (glm::vec3 *)calloc(vertCount * 2, sizeof(glm::vec3));
    glm::vec3 *tan2 = tan1 + vertCount;

    for (int i = 0; i < indCount / 3; i++) {
        long i1 = indices[i * 3];
        long i2 = indices[i * 3 + 1];
        long i3 = indices[i * 3 + 2];

        glm::vec3 pos0 = { vertices[i1], vertices[i1 + 1], vertices[i1 + 2] };
        glm::vec3 pos1 = { vertices[i2], vertices[i2 + 1], vertices[i2 + 2] };
        glm::vec3 pos2 = { vertices[i3], vertices[i3 + 1], vertices[i3 + 2] };

        glm::vec2 tex0 = { vertices[i1 + 10], vertices[i1 + 11] };
        glm::vec2 tex1 = { vertices[i2 + 10], vertices[i2 + 11] };
        glm::vec2 tex2 = { vertices[i3 + 10], vertices[i3 + 11] };

        float edge1x = pos1[0] - pos0[0];
        float edge1y = pos1[1] - pos0[1];
        float edge1z = pos1[2] - pos0[2];

        float edge2x = pos2[0] - pos0[0];
        float edge2y = pos2[1] - pos0[1];
        float edge2z = pos2[2] - pos0[2];

        float u1 = tex1[0] - tex0[0];
        float v1 = tex1[1] - tex0[1];

        float u2 = tex2[0] - tex0[0];
        float v2 = tex2[1] - tex0[1];

        float r = 1.0F / (u1 * v2 - u2 * v1);

        glm::vec3 tangent = { (v2 * edge1x - v1 * edge2x) * r, (v2 * edge1y - v1 * edge2y) * r,
                              (v2 * edge1z - v1 * edge2z) * r };
        // glm::vec3 bitangent = { (u1 * edge2x - u2 * edge1x) * r, (u1 * edge2y - u2 * edge1y) * r, (u1 * edge2z - u2 *
        // edge1z) * r };

        tan1[i1] = tan1[i1] + tangent;
        tan1[i2] = tan1[i2] + tangent;
        tan1[i3] = tan1[i3] + tangent;

        tan2[i1] = tan2[i1] + tangent;
        tan2[i2] = tan2[i2] + tangent;
        tan2[i3] = tan2[i3] + tangent;
    }

    for (int i = 0; i < vertCount; i++) {
        glm::vec3 n = { vertices[i * 12 + 3], vertices[i * 12 + 4], vertices[i * 12 + 5] };
        glm::vec3 t = { tan1[i][0], tan1[i][1], tan2[i][2] };

        // Gram-Schmidt orthogonalize
        glm::vec3 tangent = n * glm::dot(n, t);
        tangent = t - tangent;
        tangent = glm::normalize(tangent);
        vertices[i * 12 + 6] = tangent[0];
        vertices[i * 12 + 7] = tangent[1];
        vertices[i * 12 + 8] = tangent[2];

        // Calculate handedness
        glm::vec3 c = glm::cross(n, t);
        vertices[i * 12 + 9] = (glm::dot(c, tan2[i]) < 0.0F) ? -1.0F : 1.0F;
    }
    free(tan1);
}
}  // namespace neith
