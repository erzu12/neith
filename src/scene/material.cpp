#include "material.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "render/shaders.h"
#include "dataStructures.h"
#include "render/textures.h"
#include "defaults.h"

namespace neith {
    Materials::Materials(int materialCount) {
        materialCount++;
        shaders = (int*)malloc(materialCount * sizeof(int));
        textureCounts = (int*)calloc(materialCount, sizeof(int));
        textures = (int**)Alloc2DArr(materialCount, 16, sizeof(int));
        names = (char**)Alloc2DArr(materialCount, 128, sizeof(char));

        unsigned int defaultShader = LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");
        SetShader(0, defaultShader);
        for(int i = 0; i < materialCount; i++) {
            shaders[i] = defaultShader;
        }
        this->materialCount = 1;
    }

    void Materials::SetShader(int material, int shader) {
        shaders[material] = shader;
    }

    int Materials::SetShaderByName(char *materialName, int shader) {
        for(int i = 0; i < materialCount; i++) {
            if(strcmp(materialName, names[i]) == 0) {
                SetShader(i, shader);
                return 1;
            }
        }
        printf("warning: no material with name %s\n", materialName);
        return 0;
    }

    void Materials::SetTexture(int material, int texture, char *bindingName) {
        glUseProgram(shaders[material]);
        textures[material][textureCounts[material]] = texture; 
        glUniform1i(glGetUniformLocation(shaders[material], bindingName),
                    textureCounts[material]);
        textureCounts[material]++;
    }

    int Materials::SetTextureByName(char *materialName, int texture, char *bindingName) {
        for(int i = 0; i < materialCount; i++) {
            if(strcmp(materialName, names[i]) == 0) {
                SetTexture(i, texture, bindingName);
                return 1;
            }
        }
        printf("warning: no material with name %s\n", materialName);
        return 0;
    }

    void Materials::SetValueByNameF(char *materialName, char *bindingName, float value) {
        unsigned int texture = CreatValueTextureF(value);
        SetTextureByName(materialName, texture, bindingName);
    }

    void Materials::SetValueByNameV3(char *materialName, char *bindingName, float *value) {
        unsigned int texture = CreatValueTextureV3(value);
        SetTextureByName(materialName, texture, bindingName);
    }

    void Materials::SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z) {
        unsigned int texture = CreatValueTextureV3v(x, y, z);
        SetTextureByName(materialName, texture, bindingName);
    }

    Materials::~Materials() {
        free(shaders);
        free(textureCounts);
        Del2DArr((void**)names);
        Del2DArr((void**)textures);
    }

}
