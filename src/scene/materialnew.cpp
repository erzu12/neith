#include "materialnew.h"
#include "render/textures.h"

namespace neith {

MaterialNew::MaterialNew(Shader shaderProgram) {
    mShader = shaderProgram;
}

unsigned int MaterialNew::setTexture(std::string bindingName, unsigned int texture) {
    unsigned int textureSlot = mShader.bindTextureSlot(bindingName);
    mTextures[textureSlot] = texture;
    mMaxTextureSlot = std::max(mMaxTextureSlot, textureSlot);
    return textureSlot;
}

void MaterialNew::setValue(std::string bindingName, float value) {
    unsigned int texture = texture::CreatValueTextureF(value);
    setTexture(bindingName, texture);
}

void MaterialNew::setValue(std::string bindingName, float *value) {
    unsigned int texture = texture::CreatValueTextureV3(value);
    setTexture(bindingName, texture);
}

void MaterialNew::setValue(std::string bindingName, float x, float y, float z) {
    unsigned int texture = texture::CreatValueTextureV3v(x, y, z);
    setTexture(bindingName, texture);
}

Shader *MaterialNew::getShader() {
    return &mShader;
}
unsigned int MaterialNew::getMaxTextureSlot() {
    return mMaxTextureSlot;
}

unsigned int MaterialNew::getTexture(unsigned int index) {
    return mTextures[index];
}

void MaterialNew::setTransparencyTexture(unsigned int texture) {
    unsigned int textureSlot = setTexture("transparencyTexture", texture);
    mTransparencyTexture = textureSlot;
}

unsigned int MaterialNew::getTransparencyTexture() {
    return mTransparencyTexture;
}

}  // namespace neith
