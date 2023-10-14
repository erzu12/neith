#pragma once

#include <string>

#include "render/shaders.h"

namespace neith {

class Material {
public:
    Material();
    Material(Shader shaderProgram);
    void setShader(Shader shaderProgram);
    unsigned int setTexture(std::string bindingName, unsigned int texture);
    void setValue(std::string bindingName, float value);
    void setValue(std::string bindingName, float *value);
    void setValue(std::string bindingName, float x, float y, float z);
    Shader *getShader();
    unsigned int getMaxTextureSlot();
    unsigned int getTexture(unsigned int index);

    void setBackfaceCulling(bool enabled);
    bool isBackfaced();

    static void setDepthShader(Shader shader);

    void setTransparencyTexture(unsigned int texture);
    unsigned int getTransparencyTexture();

private:
    bool mBackfaced = false;
    Shader mShader;
    static Shader mDepthShader;
    unsigned int mTextures[16];
    unsigned int mTransparencyTexture;
    unsigned int mMaxTextureSlot = 1;
};

}  // namespace neith
