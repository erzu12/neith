#include "sysMesh.h"

#include "log.h"
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
    NT_INTER_INFO("test");
    Entity::AddComponent(entityID, MeshComp::GetInstance());
    glm::mat4 modelMat = TransformComp::GetGlobalTransform(entityID);
    MeshComp::AddInstance(meshID, entityID, modelMat);
}

void AddModelToEntity(unsigned int entityID, Model *model)
{
    std::vector<unsigned int> *instaceMeshes = model->GetInstanceMeshes();
    std::vector<glm::mat4> *instanceTransform = model->GetInstanceTransform();
    std::vector<std::string> *instanceNames = model->GetInstanceNames();
    glm::mat4 modelMat = TransformComp::GetGlobalTransform(entityID);

    for (int i = 0; i < instaceMeshes->size(); i++) {
        glm::mat4 instanceModelMat = modelMat * instanceTransform->at(i);
        // NT_INTER_INFO(glm::to_string(instanceModelMat));
        unsigned int entity = Entity::AddEntity(entityID, instanceModelMat, instanceNames->at(i));
        Entity::AddComponent(entity, MeshComp::GetInstance());
        MeshComp::AddInstance(instaceMeshes->at(i), entity, instanceModelMat);
    }
}
}  // namespace system
}  // namespace neith
