#pragma once

#include <glad/glad.h>

namespace neith {
namespace texture {

unsigned int CreatValueTextureF(float value);
unsigned int CreatValueTextureV3(float* value);
unsigned int CreatValueTextureV3v(float red, float green, float blue);
unsigned int LoadTexture(const char* path, GLint colorSpace, GLint internalColorSpace);
}  // namespace texture
}  // namespace neith
