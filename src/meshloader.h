#pragma once

#include "dataStructures.h"
#include "mesh.h"
#include "scene.h"
#include "cgltf.h"
#include <stdio.h>
#include <string.h>

int ReadMaterial(struct Materials *mat, cgltf_material *material){
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

void LoadModelss(struct Scene *sc, const char* paths[], int modelCount) {
    const int VERT_SIZE = 12;
    struct StaticPrimitives *sp = sc->sp;

    char **binPaths = (char**)Alloc2DArr(modelCount, 100, sizeof(char));
    int **vertDataOffsets = (int**)Alloc2DArr(modelCount, 100, sizeof(int));
    int **indDataOffsets = (int**)Alloc2DArr(modelCount, 100, sizeof(int));
    int **vertDataSizes = (int**)Alloc2DArr(modelCount, 100, sizeof(int));
    int **indDataSizes = (int**)Alloc2DArr(modelCount, 100, sizeof(int));
    bool **indicesIsShort = (bool**)Alloc2DArr(modelCount, 100, sizeof(bool));
    int currentVertOffset = 0;
    int currentIndOffset = 0;
    int *primitivesCounts = malloc(modelCount * sizeof(int));
    int *dataSizes = malloc(modelCount * sizeof(int));

    for(int i = 0; i < modelCount; i++) {
        cgltf_options options = {0};
        cgltf_data* gltfData = NULL;
        cgltf_result result = cgltf_parse_file(&options, paths[i], &gltfData);
        if (result != cgltf_result_success) {
        	printf("faild to load model: %s\n", paths[i]);
            exit(EXIT_FAILURE);
        }

        char *lastSlash = strrchr(paths[i], '/');
        int dirPathLen = lastSlash - paths[i] + 1;
        char *binUri = gltfData->buffers->uri;
        
        strncpy(binPaths[i], paths[i], dirPathLen);
        strncpy(binPaths[i] + dirPathLen, binUri, 100 - dirPathLen); 

        int curretPrimitive = 0;

        int vertLength;
        int indLength; 

        int nodeCount = gltfData->nodes_count;

        for(int j = 0; j < nodeCount; j++) {
            if(gltfData->nodes[j].mesh == NULL)
                continue;

            int primitivesCount = gltfData->nodes[j].mesh->primitives_count;

            mat4 modelMat = GLM_MAT4_IDENTITY_INIT;
            if(gltfData->nodes[j].has_translation)
                glm_translate(modelMat, gltfData->nodes[j].translation);

            if(gltfData->nodes[j].has_rotation) {
                versor rotation = GLM_QUAT_IDENTITY_INIT;
                glm_quat_init(rotation, gltfData->nodes[j].rotation[0],
                                        gltfData->nodes[j].rotation[1],
                                        gltfData->nodes[j].rotation[2],
                                        gltfData->nodes[j].rotation[3]);
                glm_quat_rotate(modelMat, rotation, modelMat);
            }
            if(gltfData->nodes[j].has_scale)
                glm_scale(modelMat, gltfData->nodes[j].scale);

            int vertOffsets[100];
            int indOffsets[100];
            int indCounts[100];
            int vertCounts[100];
            int materials[100];

            int prim = 0;

            for(int k = 0; k < primitivesCount; k++) {
                if(gltfData->nodes[j].mesh->primitives[k].attributes_count < 4) {
                    continue;
                }

                if(strcmp(gltfData->nodes[j].mesh->primitives[k].attributes[0].name, "POSITION") != 0) {
                    printf("POSITION vertex Attribute missing on mesh %s\n",
                           gltfData->nodes[j].mesh->name);
                    continue;
                }
                if(strcmp(gltfData->nodes[j].mesh->primitives[k].attributes[1].name, "NORMAL") != 0) {
                    printf("NORMAL vertex Attribute missing on mesh %s\n",
                           gltfData->nodes[j].mesh->name); 
                    continue;
                }
                if(strcmp(gltfData->nodes[j].mesh->primitives[k].attributes[2].name, "TANGENT") != 0) {
                    printf("TANGENT vertex Attribute missing on mesh %s\n",
                           gltfData->nodes[j].mesh->name); 
                    continue;
                }
                if(strcmp(gltfData->nodes[j].mesh->primitives[k].attributes[3].name, "TEXCOORD_0") != 0) {
                    printf("TEXCOORD_0 vertex Attribute missing on mesh %s\n",
                           gltfData->nodes[j].mesh->name); 
                    continue;
                }

                indicesIsShort[i][curretPrimitive] = gltfData->nodes[j].mesh->primitives[k].
                                                     indices->component_type == cgltf_component_type_r_16u;
                vertDataOffsets[i][curretPrimitive] = gltfData->nodes[j].mesh->primitives[k].
                                                      attributes[0].data->buffer_view->offset;
                indDataOffsets[i][curretPrimitive] = gltfData->nodes[j].mesh->primitives[k].
                                                     indices->buffer_view->offset;
                                                      
                vertCounts[prim] = gltfData->nodes[j].mesh->primitives[k].attributes[0].data->count;
                indCounts[prim] = gltfData->nodes[j].mesh->primitives[k].indices->count;
                vertDataSizes[i][curretPrimitive] = vertCounts[prim] * VERT_SIZE * sizeof(float);
                vertOffsets[prim] = currentVertOffset;
                indOffsets[prim] = currentIndOffset;
                if(indicesIsShort[i][curretPrimitive]){
                    indDataSizes[i][curretPrimitive] = indCounts[prim] * sizeof(short);
                } else {
                    indDataSizes[i][curretPrimitive] = indCounts[prim] * sizeof(int);
                }
                currentVertOffset += vertDataSizes[i][curretPrimitive];
                currentIndOffset += indDataSizes[i][curretPrimitive] * 2;
                if(gltfData->nodes[j].mesh->primitives[k].material != NULL) {
                    materials[prim] = ReadMaterial(sc->mat,
                            gltfData->nodes[j].mesh->primitives[k].material->name);
                } else {
                    materials[prim] = 0;
                }
                curretPrimitive++;
                prim++;
            }
            if(prim != 0) {
                //AddStaticMesh(sp, modelMat, vertOffsets, vertCounts, indOffsets, indCounts,
                              //materials, prim);
            }
        }
        dataSizes[i] = gltfData->buffers->size;
        primitivesCounts[i] = curretPrimitive;
        vertDataOffsets[i][curretPrimitive] = indLength / 2 + indDataOffsets[i][curretPrimitive - 1];
    }
    
    int vertDataSize = currentVertOffset;
    int indDataSize = currentIndOffset;
    float *vertices = malloc(vertDataSize);
    int *indices = malloc(currentIndOffset);
    currentVertOffset = 0;
    currentIndOffset = 0;

    for(int i = 0; i < modelCount; i++) {
        int dataLength = dataSizes[i];
        FILE * pFile;
        
        pFile = fopen(binPaths[i], "rb");

        if(!pFile)
            printf("faild to Load binary file: %s\n", binPaths[i]);
        
        char *sceneData = malloc(dataLength);
        if(sceneData == NULL) {
            printf("memory error: unable to allocate sceneData");
            exit(EXIT_FAILURE);
        }

        int readResult = fread(sceneData, 1, dataLength, pFile);
        if(readResult != dataLength) {
        	printf("error reading file: %s\n", binPaths[i]);
            exit(EXIT_FAILURE);
        }
        
        fclose(pFile);
        for(int j = 0; j < primitivesCounts[i]; j++) {
            int vertSize = vertDataSizes[i][j];
            int indSize = indDataSizes[i][j];
            if(currentVertOffset + vertSize > vertDataSize) {
                printf("fatal error: vertices buffer overflow\n");
                exit(EXIT_FAILURE);
            }

            memcpy(&vertices[currentVertOffset / sizeof(float)], &sceneData[vertDataOffsets[i][j]], vertSize);
            printf("%s", indicesIsShort[i][j] ? "true\n" : "false\n");
            if(indicesIsShort[i][j]){
                for(int k = 0; k < indSize / sizeof(short); k++){
                    indices[currentIndOffset / sizeof(int) + k] = 
                        *(short*)(sceneData + indDataOffsets[i][j] + k * sizeof(short));
                }
            }
            else {
                for(int k = 0; k < indSize / sizeof(int); k++){
                    indices[currentIndOffset / sizeof(int) + k] = 
                        *(int*)(sceneData + indDataOffsets[i][j] + k * sizeof(int));
                }
            }
            currentVertOffset += vertSize;
            currentIndOffset += indSize * 2;
        }
        if(sceneData == NULL) {
            printf("memory error: unable to allocate sceneData");
            exit(EXIT_FAILURE);
        }
        free(sceneData);
    }
    //sp->vertData = vertices;
    //sp->indData = indices;
    sp->vertSize = vertDataSize;
    sp->indSize = indDataSize;

    Del2DArr((void**)binPaths);
    Del2DArr((void**)vertDataOffsets);
    Del2DArr((void**)indDataOffsets);
    Del2DArr((void**)vertDataSizes);
    Del2DArr((void**)indDataSizes);
    free(primitivesCounts);
    
    sc->sp = sp;
}

void PathToBinPath(const char *path, char* binPath, char* uri);
void ReadTransform(cgltf_node *node, mat4 modelMat);
bool CheckAtributeFormat(cgltf_primitive *primitive);
char *LoadBinFile(int dataLength, char *binPath);
float *LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount);
int *LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount);

void LoadModels(struct Scene *sc, const char* paths[], int modelCount) {
    const int VERT_SIZE = 12;
    struct StaticPrimitives *sp = sc->sp;

    for(int i = 0; i < modelCount; i++) {
        cgltf_options options = {0};
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
                int vertCount = gltfData->nodes[j].mesh->primitives[k].attributes[0].data->count;
                int indCount = gltfData->nodes[j].mesh->primitives[k].indices->count;

                float *vertices = LoadVertices(sceneData, gltfData->nodes[j].mesh->primitives[k], vertCount);
                int *indices = LoadIndices(sceneData, gltfData->nodes[j].mesh->primitives[k], indCount);

                int material = ReadMaterial(sc->mat, gltfData->nodes[j].mesh->primitives[k].material);

                AddStaticPrimitive(sp, modelMat, vertices, vertCount, indices, indCount, material);

            }
        }
    }
}

void PathToBinPath(const char *path, char* binPath, char* uri) {
    char *lastSlash = strrchr(path, '/');
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
    if(primitive->attributes_count < 4) {
        return false;
    }

    if(strcmp(primitive->attributes[0].name, "POSITION") != 0) {
        return false;
    }
    if(strcmp(primitive->attributes[1].name, "NORMAL") != 0) {
        return false;
    }
    if(strcmp(primitive->attributes[2].name, "TANGENT") != 0) {
        return false;
    }
    if(strcmp(primitive->attributes[3].name, "TEXCOORD_0") != 0) {
        return false;
    }
    return true;
}

char *LoadBinFile(int dataLength, char *binPath) {
        FILE * pFile;
        
        pFile = fopen(binPath, "rb");

        if(!pFile)
            printf("faild to Load binary file: %s\n", binPath);
        
        char *sceneData = malloc(dataLength);
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

float *LoadVertices(char *sceneData, cgltf_primitive primitive, int vertCount) {
    float *vertData = (float *)sceneData;
    float *vertices = malloc(sizeof(float) * 12 * vertCount);
    
    int posOffset = primitive.attributes[0].data->buffer_view->offset;
    int normalOffset = primitive.attributes[1].data->buffer_view->offset;
    int tangentOffset = primitive.attributes[2].data->buffer_view->offset;
    int texCoordOffset = primitive.attributes[2].data->buffer_view->offset;

    for(int i = 0; i < vertCount; i++) {
        vertices[i * 12] = vertData[posOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 1] = vertData[posOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 2] = vertData[posOffset / sizeof(float) + i * 3 + 2];

        vertices[i * 12 + 3] = vertData[normalOffset / sizeof(float) + i * 3];
        vertices[i * 12 + 4] = vertData[normalOffset / sizeof(float) + i * 3 + 1];
        vertices[i * 12 + 5] = vertData[normalOffset / sizeof(float) + i * 3 + 2];

        vertices[i * 12 + 6] = vertData[tangentOffset / sizeof(float) + i * 4];
        vertices[i * 12 + 7] = vertData[tangentOffset / sizeof(float) + i * 4 + 1];
        vertices[i * 12 + 8] = vertData[tangentOffset / sizeof(float) + i * 4 + 2];
        vertices[i * 12 + 9] = vertData[tangentOffset / sizeof(float) + i * 4 + 3];

        vertices[i * 12 + 10] = vertData[texCoordOffset / sizeof(float) + i * 2];
        vertices[i * 12 + 11] = vertData[texCoordOffset / sizeof(float) + i * 2 + 1];
    }
    return vertices;
}

int *LoadIndices(char *sceneData, cgltf_primitive primitive, int indCount) {
    int *indices = malloc(sizeof(int) * indCount);

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
