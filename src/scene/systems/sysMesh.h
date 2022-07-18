#pragma once

#include <glm/mat4x4.hpp>

#include "scene/model.h"

namespace neith {
namespace system {
int AddStaticPrimitive(
    float *vertices, int vertCount, int *indices, int indCount, unsigned int mesh, int LOD, int material);

int AddMesh(int primitivesCount);
void SetLODs(unsigned int meshID, int LODCount, float *LODDistances);
void ContinuousLODUpdate();
void UpdateLODs();

void AddMeshToEntity(unsigned int entityID, unsigned int meshID);
void AddModelToEntity(unsigned int entityID, Model *model);
}  // namespace system
}  // namespace neith
