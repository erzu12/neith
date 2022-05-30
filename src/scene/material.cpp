#include "material.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataStructures.h"
#include "defaults.h"
#include "log.h"
#include "render/shaders.h"
#include "render/textures.h"

namespace neith {
unsigned int Materials::mMaterialCount = 0;
std::vector<unsigned int> Materials::mShaders;
std::vector<unsigned int> Materials::mTextureCounts;
std::vector<unsigned int *> Materials::mTextures;
unsigned int Materials::mDepthMapMaterial;
std::unordered_map<int, std::unordered_map<std::string, int>> Materials::mBindingMap;

// Materials::Materials(int materialCount) {
// materialCount++;
// mShaders = (int*)malloc(materialCount * sizeof(int));
// mTextureCounts = (int*)calloc(materialCount, sizeof(int));
// mTextures = (int**)Alloc2DArr(materialCount, 16, sizeof(int));
// names = (char**)Alloc2DArr(materialCount, 128, sizeof(char));

// unsigned int defaultShader = LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");
// SetShader(0, defaultShader);
// for(int i = 0; i < materialCount; i++) {
// mShaders[i] = defaultShader;
//}
// this->mMaterialCount = 1;
//}

unsigned int Materials::AddMaterial()
{
    static unsigned int defaultShader =
        LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");

    mMaterialCount++;

    mShaders.push_back(defaultShader);
    mTextureCounts.push_back(1);
    mTextures.push_back(new unsigned int[16]);

    return mMaterialCount - 1;
}

void Materials::AddDepthMap(unsigned int depthMap)
{
    mDepthMapMaterial = AddMaterial();
    SetShader(mDepthMapMaterial, depthMap);
}

unsigned int Materials::GetDepthMap() { return mShaders.at(mDepthMapMaterial); }

void Materials::SetShader(unsigned int material, unsigned int shader)
{
    if (mShaders.size() < material) {
        NT_INTER_ERROR("no material with index: {}", material);
    }
    else {
        mShaders[material] = shader;
    }
}

// int Materials::SetShaderByName(char *materialName, int shader) {
// for(int i = 0; i < mMaterialCount; i++) {
// if(strcmp(materialName, names[i]) == 0) {
// SetShader(i, shader);
// return 1;
//}
//}
// printf("warning: no material with name %s\n", materialName);
// return 0;
//}

void Materials::SetTexture(unsigned int material, unsigned int texture, const char *bindingName)
{
    glUseProgram(mShaders[material]);
    int location = glGetUniformLocation(mShaders[material], bindingName);
    if (location != -1) {
        auto searchShader = mBindingMap.find(mShaders[material]);
        if (searchShader == mBindingMap.end()) {
            searchShader = mBindingMap.insert({ mShaders[material], std::unordered_map<std::string, int>() }).first;
        }
        auto searchBinding = searchShader->second.find(bindingName);
        if (searchBinding == searchShader->second.end()) {
            searchBinding = searchShader->second.insert({ bindingName, mTextureCounts.at(material) }).first;
            glUniform1i(location, mTextureCounts[material]);
            mTextures.at(material)[mTextureCounts.at(material)] = texture;
        }
        else {
            mTextures.at(material)[searchBinding->second] = texture;
        }
    }
    else {
        NT_INTER_WARN("No such binding: {} in shader: {}", bindingName, mShaders[material]);
    }
    mTextureCounts[material]++;
}

// int Materials::SetTextureByName(char *materialName, int texture, char *bindingName) {
// for(int i = 0; i < mMaterialCount; i++) {
// if(strcmp(materialName, names[i]) == 0) {
// SetTexture(i, texture, bindingName);
// return 1;
//}
//}
// printf("warning: no material with name %s\n", materialName);
// return 0;
//}

void Materials::SetValue(unsigned int material, const char *bindingName, float value)
{
    unsigned int texture = texture::CreatValueTextureF(value);
    SetTexture(material, texture, bindingName);
}

void Materials::SetValue(unsigned int material, const char *bindingName, float *value)
{
    unsigned int texture = texture::CreatValueTextureV3(value);
    SetTexture(material, texture, bindingName);
}

void Materials::SetValue(unsigned int material, const char *bindingName, float x, float y, float z)
{
    unsigned int texture = texture::CreatValueTextureV3v(x, y, z);
    SetTexture(material, texture, bindingName);
}

// void Materials::SetValueByNameF(char *materialName, char *bindingName, float value) {
// unsigned int texture = CreatValueTextureF(value);
// SetTextureByName(materialName, texture, bindingName);
//}

// void Materials::SetValueByNameV3(char *materialName, char *bindingName, float *value) {
// unsigned int texture = CreatValueTextureV3(value);
// SetTextureByName(materialName, texture, bindingName);
//}

// void Materials::SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z) {
// unsigned int texture = CreatValueTextureV3v(x, y, z);
// SetTextureByName(materialName, texture, bindingName);
//}

// Materials::~Materials() {
// free(mShaders);
// free(mTextureCounts);
// Del2DArr((void**)names);
// Del2DArr((void**)mTextures);
//}

}  // namespace neith
