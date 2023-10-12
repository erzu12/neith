#include "material.h"
#include "render/textures.h"

#include "defaults.h"

namespace neith {

Material::Material() {
    static Shader defaultShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");
    mShader = defaultShader;
}

Material::Material(Shader shaderProgram) {
    mShader = shaderProgram;
}

unsigned int Material::setTexture(std::string bindingName, unsigned int texture) {
    unsigned int textureSlot = mShader.bindTextureSlot(bindingName);
    mTextures[textureSlot] = texture;
    mMaxTextureSlot = std::max(mMaxTextureSlot, textureSlot);
    return textureSlot;
}

void Material::setValue(std::string bindingName, float value) {
    unsigned int texture = texture::CreatValueTextureF(value);
    setTexture(bindingName, texture);
}

void Material::setValue(std::string bindingName, float *value) {
    unsigned int texture = texture::CreatValueTextureV3(value);
    setTexture(bindingName, texture);
}

void Material::setValue(std::string bindingName, float x, float y, float z) {
    unsigned int texture = texture::CreatValueTextureV3v(x, y, z);
    setTexture(bindingName, texture);
}

Shader *Material::getShader() {
    return &mShader;
}
unsigned int Material::getMaxTextureSlot() {
    return mMaxTextureSlot;
}

unsigned int Material::getTexture(unsigned int index) {
    return mTextures[index];
}

void Material::setTransparencyTexture(unsigned int texture) {
    unsigned int textureSlot = setTexture("transparencyTexture", texture);
    mTransparencyTexture = textureSlot;
}

unsigned int Material::getTransparencyTexture() {
    return mTransparencyTexture;
}

}  // namespace neith
