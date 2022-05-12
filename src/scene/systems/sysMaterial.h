#pragma once

namespace neith {
namespace system {
void SetShader(unsigned int meshID, int material, int shader);
void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName);

void SetValue(unsigned int meshID, int material, const char *bindingName, float value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float *value);
void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z);
}  // namespace system
}  // namespace neith
