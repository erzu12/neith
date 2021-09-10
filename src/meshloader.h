#pragma once

#include "mesh.h"
#include "cgltf.h"
#include <stdio.h>
#include <string.h>

void** Alloc2DArr(int x, int y, int typeSize) {
    void **arr = malloc(x * sizeof(void*));
    char* arr2 = malloc((size_t)(y * x * typeSize));
    if (arr == NULL || arr2 == NULL) {
        printf("2Darray alloc error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < x; i++) {
        arr[i] = arr2 + (i * y) * typeSize;
    }
    return arr;
}

void Del2DArr(void **arr) {
    free(arr[0]);
    free(arr);
}

mat4 *GetTransforms(int meshCount, cgltf_data *gltfData) {
    mat4 *modelMats = malloc(meshCount * sizeof(mat4));
    if (modelMats == NULL) {
        printf("modelMats alloc error");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < meshCount; i++) {
        
    }
    return modelMats; 
}


struct StaticPrimitives* LoadModels(const char* paths[], int modelCount) {
    const int VERT_SIZE = 12;

    struct StaticPrimitives* sp = InitStaticPrimitives(modelCount * 100);
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
        int mesh = 0;

        for(int j = 0; j < nodeCount; j++) {
            if(gltfData->nodes[j].mesh == NULL)
                continue;

            int primitivesCount = gltfData->meshes[j].primitives_count;

            mat4 modelMat = GLM_MAT4_IDENTITY_INIT;
            if(gltfData->nodes[j].has_translation)
                glm_translate(modelMat, gltfData->nodes[j].translation);
            if(gltfData->nodes[j].has_rotation)
                //glm_quat_rotate(modelMat, gltfData->nodes[j].rotation, modelMat);
            if(gltfData->nodes[j].has_scale)
                glm_scale(modelMat, gltfData->nodes[j].scale);

            int vertOffsets[100];
            int indOffsets[100];
            int indCounts[100];
            int vertCounts[100];

            for(int k = 0; k < primitivesCount; k++) {
                if(strcmp(gltfData->meshes[mesh].primitives[k].attributes[0].name, "POSITION") != 0)
                    printf("POSITION vertex Attribute missing on mesh %s\n",
                           gltfData->meshes[mesh].name); 
                if(strcmp(gltfData->meshes[mesh].primitives[k].attributes[1].name, "NORMAL") != 0)
                    printf("NORMAL vertex Attribute missing on mesh %s\n",
                           gltfData->meshes[mesh].name); 
                if(strcmp(gltfData->meshes[mesh].primitives[k].attributes[2].name, "TANGENT") != 0)
                    printf("TANGENT vertex Attribute missing on mesh %s\n",
                           gltfData->meshes[mesh].name); 
                if(strcmp(gltfData->meshes[mesh].primitives[k].attributes[3].name, "TEXCOORD_0") != 0)
                    printf("TEXCOORD_0 vertex Attribute missing on mesh %s\n",
                           gltfData->meshes[mesh].name); 

                indicesIsShort[i][curretPrimitive] = gltfData->meshes[mesh].primitives[k].
                                                     indices->component_type == cgltf_component_type_r_16u;
                vertDataOffsets[i][curretPrimitive] = gltfData->meshes[mesh].primitives[k].
                                                      attributes[0].data->buffer_view->offset;
                indDataOffsets[i][curretPrimitive] = gltfData->meshes[mesh].primitives[k].
                                                     indices->buffer_view->offset;
                                                      
                vertCounts[k] = gltfData->meshes[mesh].primitives[k].attributes[0].data->count;
                indCounts[k] = gltfData->meshes[mesh].primitives[k].indices->count;
                vertDataSizes[i][curretPrimitive] = vertCounts[k] * VERT_SIZE * sizeof(float);
                vertOffsets[k] = currentVertOffset;
                indOffsets[k] = currentIndOffset;
                if(indicesIsShort[i][curretPrimitive]){
                    indDataSizes[i][curretPrimitive] = indCounts[k] * sizeof(short);
                } else {
                    indDataSizes[i][curretPrimitive] = indCounts[k] * sizeof(int);
                }
                currentVertOffset += vertDataSizes[i][curretPrimitive];
                currentIndOffset += indDataSizes[i][curretPrimitive] * 2;
                curretPrimitive++;
            }
            AddStaticMesh(sp, modelMat, vertOffsets, vertCounts, indOffsets, indCounts, primitivesCount);
            mesh++;
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
    sp->vertData = vertices;
    sp->indData = indices;
    sp->vertSize = vertDataSize;
    sp->indSize = indDataSize;

    Del2DArr((void**)binPaths);
    Del2DArr((void**)vertDataOffsets);
    Del2DArr((void**)indDataOffsets);
    Del2DArr((void**)vertDataSizes);
    Del2DArr((void**)indDataSizes);
    free(primitivesCounts);
    
    return sp;
}

