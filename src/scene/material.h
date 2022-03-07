#pragma once

#include <vector>

namespace neith {
    class Materials {
    public:
        static int mMaterialCount;
        static std::vector<int> mShaders;
        //char **names;
        static std::vector<int> mTextureCounts;
        static std::vector<int*> mTextures;

        //Materials(int materialCount);

        static int AddMaterial();

        static void SetShader(int material, int shader);

        //int SetShaderByName(char *materialName, int shader);

        static void SetTexture(int material, int texture, char *bindingName);

        //int SetTextureByName(char *materialName, int texture, char *bindingName);

        static void SetValue(int material, char *bindingName, float value);

        //void SetValueByNameF(char *materialName, char *bindingName, float value);

        //void SetValueByNameV3(char *materialName, char *bindingName, float *value);

        //void SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z);

        ~Materials();
    };
}
