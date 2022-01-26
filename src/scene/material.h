#pragma once

namespace neith {
    class Materials {
    public:
        int materialCount;
        int *shaders;
        char **names;
        int *textureCounts;
        int **textures;

        Materials(int materialCount);

        void SetShader(int material, int shader);

        int SetShaderByName(char *materialName, int shader);

        void SetTexture(int material, int texture, char *bindingName);

        int SetTextureByName(char *materialName, int texture, char *bindingName);

        void SetValueByNameF(char *materialName, char *bindingName, float value);

        void SetValueByNameV3(char *materialName, char *bindingName, float *value);

        void SetValueByNameV3v(char *materialName, char *bindingName, float x, float y, float z);

        ~Materials();
    };
}
