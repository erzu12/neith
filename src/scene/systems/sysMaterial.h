#pragma once

#include "scene/model.h"

namespace neith {
namespace system {
void SetShader(unsigned int meshID, int material, int shader);
void SetShader(Model *model, int material, int shader);
void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName);
void SetTexture(Model *model, int material, int texture, const char *bindingName);

void SetValue(unsigned int meshID, int material, const char *bindingName, float value);
void SetValue(Model *model, int material, const char *bindingName, float value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float *value);
void SetValue(Model *model, int material, const char *bindingName, float *value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z);
void SetValue(Model *model, int material, const char *bindingName, float x, float y, float z);
}  // namespace system
}  // namespace neith
