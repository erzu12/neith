#pragma once

#include "shaders.h"
#include "dataStructures.h"
#include "textures.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

#define ASSET_DIR "/media/ssd2/dev/neith/assets/"

struct Materials {
    int materialCount;
    int *shaders;
    char **names;
    int *textureCounts;
    int **textures;
};

void SetShader(struct Materials *mat, int material, int shader) {
    mat->shaders[material] = shader;
}

int SetShaderByName(struct Materials *mat, char *materialName, int shader) {
    for(int i = 0; i < mat->materialCount; i++) {
        if(strcmp(materialName, mat->names[i]) == 0) {
            SetShader(mat, i, shader);
            return 1;
        }
    }
    printf("warning: no material with name %s\n", materialName);
    return 0;
}

void SetTexture(struct Materials *mat, int material, int texture, char *bindingName) {
    glUseProgram(mat->shaders[material]);
    mat->textures[material][mat->textureCounts[material]] = texture; 
    glUniform1i(glGetUniformLocation(mat->shaders[material], bindingName),
                mat->textureCounts[material]);
    mat->textureCounts[material]++;
}

int SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName) {
    for(int i = 0; i < mat->materialCount; i++) {
        if(strcmp(materialName, mat->names[i]) == 0) {
            SetTexture(mat, i, texture, bindingName);
            return 1;
        }
    }
    printf("warning: no material with name %s\n", materialName);
    return 0;
}

void SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value) {
    unsigned int texture = CreatValueTextureF(value);
    SetTextureByName(mat, materialName, texture, bindingName);
}

void SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value) {
    unsigned int texture = CreatValueTextureV3(value);
    SetTextureByName(mat, materialName, texture, bindingName);
}

void SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z) {
    unsigned int texture = CreatValueTextureV3v(x, y, z);
    SetTextureByName(mat, materialName, texture, bindingName);
}

struct Materials *InitMaterials(int materialCount) {
    materialCount++;
    struct Materials *mat = malloc(sizeof(struct Materials));
    mat->shaders = malloc(materialCount * sizeof(int));
    mat->textureCounts = calloc(materialCount, sizeof(int));
    mat->textures = (int**)Alloc2DArr(materialCount, 16, sizeof(int));
    mat->names = (char**)Alloc2DArr(materialCount, 128, sizeof(char));

    unsigned int defaultShader = LoadAndCompileShaders(ASSET_DIR "default.vert", ASSET_DIR "default.frag");
    SetShader(mat, 0, defaultShader);
    for(int i = 0; i < materialCount; i++) {
        mat->shaders[i] = defaultShader;
    }
    mat->materialCount = 1;

    return mat;
}

void DeleteMaterials(struct Materials *mat) {
    free(mat->shaders);
    free(mat->textureCounts);
    Del2DArr((void**)mat->names);
    Del2DArr((void**)mat->textures);
    free(mat);
}

