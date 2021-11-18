#pragma once

#define ASSET_DIR "/media/home/dev/neith/assets/"

struct Materials {
    int materialCount;
    int *shaders;
    char **names;
    int *textureCounts;
    int **textures;
};

void SetShader(struct Materials *mat, int material, int shader);

int SetShaderByName(struct Materials *mat, char *materialName, int shader);

void SetTexture(struct Materials *mat, int material, int texture, char *bindingName);

int SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName);

void SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value);

void SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value);

void SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z);

struct Materials *InitMaterials(int materialCount);

void DeleteMaterials(struct Materials *mat);
