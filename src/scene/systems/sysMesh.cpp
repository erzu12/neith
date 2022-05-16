#include "sysMesh.h"

#include "scene/components/meshComp.h"
#include "scene/components/transformComp.h"
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

void AddMeshToEntity(unsigned int entityID, unsigned int meshID)
{
    Entity::AddComponent(entityID, "Mesh");
    glm::mat4 modelMat = TransformComp::GetGlobalTransform(entityID);
    MeshComp::AddInstance(meshID, entityID, modelMat);
}
}  // namespace system
}  // namespace neith
