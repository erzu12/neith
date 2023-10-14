#include "material.h"
#include "log.h"
#include "render/textures.h"

#include "defaults.h"

namespace neith {

Shader Material::mDepthShader;

Material::Material() {
    static Shader defaultShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "default.vert", NTH_ASSET_DIR "default.frag");
    static unsigned int defaultTexture = texture::CreatValueTextureV3v(1.0f, 0.0f, 1.0f);
    NT_INTER_INFO("default texture: {}", defaultTexture);
    mShader = defaultShader;
    for (int i = 0; i < 16; i++) {
        mTextures[i] = defaultTexture;
    }
}

Material::Material(Shader shaderProgram) {
    mShader = shaderProgram;
}

void Material::setShader(Shader shaderProgram) {
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
    NT_INTER_INFO("set value: {}, {}, {} texture: {}", x,y,z, texture);
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

void Material::setDepthShader(Shader shader) {
    mDepthShader = shader;
}

void Material::setTransparencyTexture(unsigned int texture) {
    unsigned int textureSlot = setTexture("transparencyTexture", texture);
    mTransparencyTexture = textureSlot;
}

unsigned int Material::getTransparencyTexture() {
    return mTransparencyTexture;
}

}  // namespace neith
