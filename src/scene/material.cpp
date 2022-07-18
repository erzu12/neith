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
std::vector<unsigned int> Materials::mTransparencyTexture;
std::vector<bool> Materials::mBackfaced;
unsigned int Materials::mDepthMapShader;
std::unordered_map<int, std::unordered_map<std::string, int>> Materials::mBindingMap;
std::unordered_map<unsigned int, int> Materials::mBindingCounts;

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
    static unsigned int defaultTexture = texture::CreatValueTextureV3v(1.0f, 0.0f, 1.0f);

    mMaterialCount++;

    mShaders.push_back(defaultShader);
    mBindingCounts.insert({ defaultShader, 1 });
    mTextureCounts.push_back(1);
    mBackfaced.push_back(false);
    unsigned int *textures = new unsigned int[16];
    for (int i = 0; i < 16; i++) {
        textures[i] = defaultTexture;
    }
    mTextures.push_back(textures);
    mTransparencyTexture.push_back(0);

    return mMaterialCount - 1;
}

void Materials::SetShader(unsigned int material, unsigned int shader)
{
    mBindingCounts.insert({ shader, 1 });
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

int Materials::SetTexture(unsigned int material, unsigned int texture, const char *bindingName)
{
    glUseProgram(mShaders[material]);
    int binding;
    int location = glGetUniformLocation(mShaders[material], bindingName);
    if (location != -1) {
        auto searchShader = mBindingMap.find(mShaders[material]);
        if (searchShader == mBindingMap.end()) {
            searchShader = mBindingMap.insert({ mShaders[material], std::unordered_map<std::string, int>() }).first;
        }
        auto searchBinding = searchShader->second.find(bindingName);
        if (searchBinding == searchShader->second.end()) {
            binding = mBindingCounts.at(mShaders.at(material));
            searchBinding = searchShader->second.insert({ bindingName, binding }).first;
            glUniform1i(location, binding);
            mTextures.at(material)[binding] = texture;

            mBindingCounts.at(mShaders.at(material))++;
            mTextureCounts.at(material) = binding + 1;
        }
        else {
            binding = searchBinding->second;
            mTextures.at(material)[binding] = texture;
        }
    }
    else {
    }
    return binding;
}

void Materials::SetTransparancyTexture(unsigned int material, unsigned int texture, const char *bindingName)
{
    int binding = SetTexture(material, texture, bindingName);
    mTransparencyTexture.at(material) = binding;
}

void Materials::SetDepthMapShader(unsigned int depthMapShader)
{
    mDepthMapShader = depthMapShader;

    glUseProgram(mDepthMapShader);
    int location = glGetUniformLocation(mDepthMapShader, "alpha");
    glUniform1i(location, 1);
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
