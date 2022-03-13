#include "material.h"

#include "log.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "render/shaders.h"
#include "dataStructures.h"
#include "render/textures.h"
#include "defaults.h"

namespace neith {
    int Materials::mMaterialCount = 0;
    std::vector<int> Materials::mShaders;
    std::vector<int> Materials::mTextureCounts;
    std::vector<int*> Materials::mTextures;

    //Materials::Materials(int materialCount) {
        //materialCount++;
        //mShaders = (int*)malloc(materialCount * sizeof(int));
        //mTextureCounts = (int*)calloc(materialCount, sizeof(int));
        //mTextures = (int**)Alloc2DArr(materialCount, 16, sizeof(int));
        //names = (char**)Alloc2DArr(materialCount, 128, sizeof(char));

        //unsigned int defaultShader = LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");
        //SetShader(0, defaultShader);
        //for(int i = 0; i < materialCount; i++) {
            //mShaders[i] = defaultShader;
        //}
        //this->mMaterialCount = 1;
    //}

    int Materials::AddMaterial() {
        static unsigned int defaultShader = LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");

        mMaterialCount++;

        mShaders.push_back(defaultShader);
        mTextureCounts.push_back(0);
        mTextures.push_back(new int[16]);

        return mMaterialCount - 1;    
    }

    void Materials::SetShader(int material, int shader) {
        if(mShaders.size() < material) {
            NT_INTER_ERROR("no material with index: {}",  material);
        }
        else {
            mShaders[material] = shader;
        }
    }

    //int Materials::SetShaderByName(char *materialName, int shader) {
        //for(int i = 0; i < mMaterialCount; i++) {
            //if(strcmp(materialName, names[i]) == 0) {
                //SetShader(i, shader);
                //return 1;
            //}
        //}
        //printf("warning: no material with name %s\n", materialName);
        //return 0;
    //}

    void Materials::SetTexture(int material, int texture, const char *bindingName) {
        glUseProgram(mShaders[material]);
        mTextures.at(material)[mTextureCounts.at(material)] = texture; 
        glUniform1i(glGetUniformLocation(mShaders[material], bindingName), mTextureCounts[material]);
        mTextureCounts[material]++;
    }

    //int Materials::SetTextureByName(char *materialName, int texture, char *bindingName) {
        //for(int i = 0; i < mMaterialCount; i++) {
            //if(strcmp(materialName, names[i]) == 0) {
                //SetTexture(i, texture, bindingName);
                //return 1;
            //}
        //}
        //printf("warning: no material with name %s\n", materialName);
        //return 0;
    //}

    void Materials::SetValue(int material, const char *bindingName, float value) {
        unsigned int texture = CreatValueTextureF(value);
        SetTexture(material, texture, bindingName);
    }

    void Materials::SetValue(int material, const char *bindingName, float *value) {
        unsigned int texture = CreatValueTextureV3(value);
        SetTexture(material, texture, bindingName);
    }

    void Materials::SetValue(int material, const char *bindingName, float x, float y, float z) {
        unsigned int texture = CreatValueTextureV3v(x, y, z);
        SetTexture(material, texture, bindingName);
    }

    //void Materials::SetValueByNameF(char *materialName, char *bindingName, float value) {
        //unsigned int texture = CreatValueTextureF(value);
        //SetTextureByName(materialName, texture, bindingName);
    //}

    //void Materials::SetValueByNameV3(char *materialName, char *bindingName, float *value) {
        //unsigned int texture = CreatValueTextureV3(value);
        //SetTextureByName(materialName, texture, bindingName);
    //}

    //void Materials::SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z) {
        //unsigned int texture = CreatValueTextureV3v(x, y, z);
        //SetTextureByName(materialName, texture, bindingName);
    //}

    //Materials::~Materials() {
        //free(mShaders);
        //free(mTextureCounts);
        //Del2DArr((void**)names);
        //Del2DArr((void**)mTextures);
    //}

}
