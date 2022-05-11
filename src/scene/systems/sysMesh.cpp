#include "sysMesh.h"

#include "scene/components/compManager.h"
#include "scene/components/meshComp.h"
#include "scene/scene.h"

namespace neith {
namespace system {
int AddStaticPrimitive(float *vertices, int vertCount, int *indices, int indCount, int material)
{
    // Scene::GetEntity(entity)->AddComponentByID(CompManager::GetCompID<MeshComp>());
    // return MeshComp::AddStaticPrimitive(entity, modelMat, vertices, vertCount, indices, indCount, material);
    return MeshComp::AddStaticPrimitive(vertices, vertCount, indices, indCount, material);
}

int AddMesh(int primitivesCount) { return MeshComp::AddStaticMesh(primitivesCount); }

void AddMeshToEntity(unsigned int entityID, unsigned int meshID, glm::mat4 modelMat)
{
    Scene::GetEntity(entityID)->AddComponentByID(CompManager::GetCompID<MeshComp>());
    MeshComp::AddInstance(meshID, entityID, modelMat);
}
}  // namespace system
}  // namespace neith
