#pragma once

#include <string>

#include "render/shaders.h"

namespace neith {

class MaterialNew {
public:
    MaterialNew(Shader shaderProgram);
    unsigned int setTexture(std::string bindingName, unsigned int texture);
    void setValue(std::string bindingName, float value);
    void setValue(std::string bindingName, float *value);
    void setValue(std::string bindingName, float x, float y, float z);
    Shader *getShader();
    unsigned int getMaxTextureSlot();
    unsigned int getTexture(unsigned int index);

    void setTransparencyTexture(unsigned int texture);
    unsigned int getTransparencyTexture();

private:
    Shader mShader;
    unsigned int mTextures[16];
    unsigned int mTransparencyTexture;
    unsigned int mMaxTextureSlot = 0;
};

}  // namespace neith
