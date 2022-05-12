#include "sysMaterial.h"

#include "scene/components/meshComp.h"
#include "scene/material.h"

namespace neith {
namespace system {
void SetShader(unsigned int meshID, int material, int shader)
{
    Materials::SetShader(MeshComp::GetMaterial(meshID, material), shader);
}

void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName)
{
    Materials::SetTexture(MeshComp::GetMaterial(meshID, material), texture, bindingName);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float value)
{
    Materials::SetValue(MeshComp::GetMaterial(meshID, material), bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float *value)
{
    Materials::SetValue(MeshComp::GetMaterial(meshID, material), bindingName, value);
}

void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z)
{
    Materials::SetValue(MeshComp::GetMaterial(meshID, material), bindingName, x, y, z);
}
}  // namespace system
}  // namespace neith