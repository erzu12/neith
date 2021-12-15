#include "meshloader.h"

#include "dataStructures.h"
#include "mesh.h"
#include "scene.h"
#include "material.h"
#include <cgltf.h>
#include <stdio.h>
#include <string.h>


void LoadModels(struct Scene *sc, const char* paths[], int modelCount) {
    const int VERT_SIZE = 12;
    StaticPrimitives *sp = sc->sp;

    for(int i = 0; i < modelCount; i++) {
        cgltf_options options = {(cgltf_file_type)0};
        cgltf_data* gltfData = NULL;
        cgltf_result result = cgltf_parse_file(&options, paths[i], &gltfData);
        if (result != cgltf_result_success) {
        	printf("faild to load model: %s\n", paths[i]);
            exit(EXIT_FAILURE);
        }

        char binPath[1024];
        PathToBinPath(paths[i], binPath, gltfData->buffers->uri);

        int dataSize = gltfData->buffers->size;
        char *sceneData = LoadBinFile(dataSize, binPath);

        int curretPrimitive = 0;

        int vertLength;
        int indLength; 

        int nodeCount = gltfData->nodes_count;

        for(int j = 0; j < nodeCount; j++) {
            if(gltfData->nodes[j].mesh == NULL)
                continue;

            int primitivesCount = gltfData->nodes[j].mesh->primitives_count;

            mat4 modelMat = GLM_MAT4_IDENTITY_INIT;
            ReadTransform(&gltfData->nodes[j], modelMat);

            for(int k = 0; k < primitivesCount; k++) {
                if(!CheckAtributeFormat(&gltfData->nodes[j].mesh->primitives[k])){
                    printf("WARNING: Vertex Attribute missing on mesh %s\n", gltfData->nodes[j].mesh->name);
                    continue;
                }
                bool hasTangents = HasTangents(&gltfData->nodes[j].mesh->primitives[k]); 

                int vertCount = gltfData->nodes[j].mesh->primitives[k].attributes[0].data->count;
                int indCount = gltfData->nodes[j].mesh->primitives[k].indices->count;

                float *vertices = LoadVertices(sceneData, gltfData->nodes[j].mesh->primitives[k], vertCount, hasTangents);
                int *indices = LoadIndices(sceneData, gltfData->nodes[j].mesh->primitives[k], indCount);
                if(!hasTangents)
                    CalcTangents(vertices, vertCount, indices, indCount);

                int material = ReadMaterial(sc->mat, gltfData->nodes[j].mesh->primitives[k].material);

                sp->AddStaticPrimitive(modelMat, vertices, vertCount, indices, indCount, material);
            }
        }
    }
}

void PathToBinPath(const char *path, char* binPath, char* uri) {
    char *lastSlash = (char*)strrchr(path, '/');
    int dirPathLen = lastSlash - path + 1;
    
    strncpy(binPath, path, dirPathLen);
    strncpy(binPath + dirPathLen, uri, 1024 - dirPathLen); 
}

void ReadTransform(cgltf_node *node, mat4 modelMat) {
    if(node->has_translation)
        glm_translate(modelMat, node->translation);

    if(node->has_rotation) {
        versor rotation = GLM_QUAT_IDENTITY_INIT;
        glm_quat_init(rotation, node->rotation[0],
                                node->rotation[1],
                                node->rotation[2],
                                node->rotation[3]);
        glm_quat_rotate(modelMat, rotation, modelMat);
    }
    if(node->has_scale)
        glm_scale(modelMat, node->scale);
}

bool CheckAtributeFormat(cgltf_primitive *primitive) {
    // refactor this garbage
    if(primitive->attributes_count < 3) return false;

    if(strcmp(primitive->attributes[0].name, "POSITION") != 0) return false;
    if(strcmp(primitive->attributes[1].name, "NORMAL") != 0) return false;
    if(strcmp(primitive->attributes[2].name, "TEXCOORD_0") != 0) {
        if(primitive->attributes_count < 4) return false;
        if(strcmp(primitive->attributes[3].name, "TEXCOORD_0") != 0) return false;
    }
    return true;
}

bool HasTangents(cgltf_primitive *primitive) {
    if(strcmp(primitive->attributes[2].name, "TANGENT") != 0) return false;
    return true;
}

int ReadMaterial(Materials *mat, cgltf_material *material) {
    if(material == NULL) 
        return 0;

    int matInd = mat->materialCount;
    for(int i = 0; i < matInd; i++) {
        if(strcmp(material->name, mat->names[i]) == 0) {
            return i;
        }
    }
    strncpy(mat->names[matInd], material->name, 128);
    mat->materialCount++;

    return matInd;
}

char *LoadBinFile(int dataLength, char *binPath) {
    FILE * pFile;
    
    pFile = fopen(binPath, "rb");

    if(!pFile) {
        printf("faild to Load binary file: %s\n", binPath);
        exit(EXIT_FAILURE);
    }
    
    char *sceneData = (char*)malloc(dataLength);
    if(sceneData == NULL) {
        printf("memory error: unable to allocate sceneData");
        exit(EXIT_FAILURE);
    }

    int readResult = fread(sceneData, 1, dataLength, pFile);
    if(readResult != dataLength) {
        printf("error reading file: %s\n", binPath);
        exit(EXIT_FAILURE);
    }
    
    fclose(pFile);

    return sceneData;
}

float *LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount, bool hasTangents) {
    float *vertData = (float *)sceneData;
    float *vertices = (float*)malloc(sizeof(float) * 12 * vertCount);
    
    int posOffset = primitive.attributes[0].data->buffer_view->offset;
    int normalOffset = primitive.attributes[1].data->buffer_view->offset;
    int tangentOffset, texCoordOffset;
    if(hasTangents) {
        tangentOffset = primitive.attributes[2].data->buffer_view->offset;
        texCoordOffset = primitive.attributes[3].data->buffer_view->offset;
    }
    else {
        texCoordOffset = primitive.attributes[2].data->buffer_view->offset;
    }

    for(int i = 0; i < vertCount; i++) {
        vertices[i * 12] = vertData[posOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 1] = vertData[posOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 2] = vertData[posOffset / sizeof(float) + i * 3 + 2];

        vertices[i * 12 + 3] = vertData[normalOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 4] = vertData[normalOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 5] = vertData[normalOffset / sizeof(float) + i * 3 + 2];

        if(hasTangents) { 
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

int *LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount) {
    int *indices = (int*)malloc(sizeof(int) * indCount);

    int indOffset = primitive.indices->buffer_view->offset;

    if(primitive.indices->component_type == cgltf_component_type_r_16u) {
        unsigned short *indData = (unsigned short *)&sceneData[indOffset];
        for(int i = 0; i < indCount; i++){
            indices[i] = indData[i];
        }
    }
    else {
        unsigned int *indData = (unsigned int *)&sceneData[indOffset];
        for(int i = 0; i < indCount; i++){
            indices[i] = indData[i];
        }
    }
    return indices;
}

void CalcTangents(float *vertices, int vertCount, int *indices, int indCount) {
    vec3 *tan1 = (vec3*)calloc(vertCount * 2, sizeof(vec3));
    vec3 *tan2 = tan1 + vertCount;
    
    for (int i = 0; i < indCount / 3; i++)
    {
        long i1 = indices[i * 3];
        long i2 = indices[i * 3 + 1];
        long i3 = indices[i * 3 + 2];
        
        vec3 pos0 = {vertices[i1], vertices[i1 + 1], vertices[i1 + 2]};
        vec3 pos1 = {vertices[i2], vertices[i2 + 1], vertices[i2 + 2]};
        vec3 pos2 = {vertices[i3], vertices[i3 + 1], vertices[i3 + 2]};
        
        vec2 tex0 = {vertices[i1 + 10], vertices[i1 + 11]};
        vec2 tex1 = {vertices[i2 + 10], vertices[i2 + 11]};
        vec2 tex2 = {vertices[i3 + 10], vertices[i3 + 11]};
        
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

        vec3 tangent = {(v2 * edge1x - v1 * edge2x) * r, (v2 * edge1y - v1 * edge2y) * r,
                (v2 * edge1z - v1 * edge2z) * r};
        vec3 bitangent = {(u1 * edge2x - u2 * edge1x) * r, (u1 * edge2y - u2 * edge1y) * r,
                (u1 * edge2z - u2 * edge1z) * r};
        
        glm_vec3_add(tan1[i1], tangent, tan1[i1]);
        glm_vec3_add(tan1[i2], tangent, tan1[i2]);
        glm_vec3_add(tan1[i3], tangent, tan1[i3]);
        
        glm_vec3_add(tan2[i1], tangent, tan2[i1]);
        glm_vec3_add(tan2[i2], tangent, tan2[i2]);
        glm_vec3_add(tan2[i3], tangent, tan2[i3]);
    }
    
    for (int i = 0; i < vertCount; i++)
    {
        vec3 n = {vertices[i * 12 + 3], vertices[i * 12 + 4], vertices[i * 12 + 5]};
        vec3 t = {tan1[i][0], tan1[i][1], tan2[i][2]};
        
        // Gram-Schmidt orthogonalize
        vec3 tangent;
        glm_vec3_scale(n, glm_vec3_dot(n, t), tangent);
        glm_vec3_sub(t, tangent, tangent);
        glm_vec3_normalize(tangent);
        vertices[i * 12 + 6] = tangent[0];
        vertices[i * 12 + 7] = tangent[1];
        vertices[i * 12 + 8] = tangent[2];
        
        // Calculate handedness
        vec3 c;
        glm_vec3_cross(n, t, c); 
        vertices[i * 12 + 9] = (glm_vec3_dot(c, tan2[i]) < 0.0F) ? -1.0F : 1.0F;
    }
    free(tan1);
}
