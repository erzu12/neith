#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <string>

namespace neith {
class Materials {
public:
    // Materials(int materialCount);

    static unsigned int AddMaterial();

    static void AddDepthMap(unsigned int depthMap);

    static unsigned int GetDepthMap();

    static void SetShader(unsigned int material, unsigned int shader);

    // int SetShaderByName(char *materialName, int shader);

    static void SetTexture(unsigned int material, unsigned int texture, const char *bindingName);

    // int SetTextureByName(char *materialName, int texture, char *bindingName);

    static void SetValue(unsigned int material, const char *bindingName, float value);

    static void SetValue(unsigned int material, const char *bindingName, float *value);

    static void SetValue(unsigned int material, const char *bindingName, float x, float y, float z);

    // void SetValueByNameF(char *materialName, char *bindingName, float value);

    // void SetValueByNameV3(char *materialName, char *bindingName, float *value);

    // void SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z);

    static unsigned int GetMaterialCount() { return mMaterialCount; }
    static unsigned int GetShader(unsigned int material) { return mShaders.at(material); }
    static unsigned int GetTextureCount(unsigned int material) { return mTextureCounts.at(material); }
    static unsigned int GetTexture(unsigned int material, unsigned int texture)
    {
        return mTextures.at(material)[texture];
    }

    ~Materials();

private:
    static unsigned int mMaterialCount;
    static std::vector<unsigned int> mShaders;
    // char **names;
    static std::vector<unsigned int> mTextureCounts;
    static std::vector<unsigned int *> mTextures;
    static std::unordered_map<int, std::unordered_map<std::string, int>> mBindingMap;
    static unsigned int mDepthMap;
};
}  // namespace neith
