#include "sysMesh.h"

#include <chrono>
#include <thread>

#include "log.h"
#include "scene/components/cameraComp.h"
#include "scene/components/meshComp.h"
#include "scene/components/transformComp.h"
#include "scene/scene.h"
#include "window/window.h"

namespace neith {
namespace system {
int AddStaticPrimitive(
    float *vertices, int vertCount, int *indices, int indCount, unsigned int mesh, int LOD, int material)
{
    // Scene::GetEntity(entity)->AddComponentByID(CompManager::GetCompID<MeshComp>());
    // return MeshComp::AddStaticPrimitive(entity, modelMat, vertices, vertCount, indices, indCount, material);
    return MeshComp::AddStaticPrimitive(vertices, vertCount, indices, indCount, mesh, LOD, material);
}

int AddMesh(int primitivesCount) { return MeshComp::AddStaticMesh(primitivesCount); }

void SetLODs(unsigned int meshID, int LODCount, float *LODDistances)
{
    MeshComp::SetLODs(meshID, LODCount, LODDistances);
}

void AddMeshToEntity(unsigned int entityID, unsigned int meshID)
{
    Entity::AddComponent(entityID, MeshComp::GetInstance());
    glm::mat4 modelMat = TransformComp::GetGlobalTransform(entityID);
    MeshComp::AddInstance(meshID, entityID, modelMat);
}

void ContinuousLODUpdate()
{
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> current =
        std::chrono::steady_clock::now();
    using interval = std::chrono::duration<double, std::ratio<1, 60>>;
    while (!Window::ShouldClose()) {
        UpdateLODs();
        current += interval(1);
        std::this_thread::sleep_until(current);
    }
}

void UpdateLODs() { MeshComp::UpdateLODs(CameraComp::GetCameraPos()); }

void AddModelToEntity(unsigned int entityID, Model *model)
{
    std::vector<unsigned int> *instaceMeshes = model->GetInstanceMeshes();
    std::vector<glm::mat4> *instanceTransform = model->GetInstanceTransform();
    std::vector<std::string> *instanceNames = model->GetInstanceNames();
    std::string entityName = Entity::GetName(entityID);
    glm::mat4 modelMat = TransformComp::GetGlobalTransform(entityID);

    for (int i = 0; i < instaceMeshes->size(); i++) {
        glm::mat4 instanceModelMat = modelMat * instanceTransform->at(i);
        // NT_INTER_INFO(glm::to_string(instanceModelMat));
        std::string name = entityName + "." + instanceNames->at(i);
        unsigned int entity = Entity::AddEntity(entityID, instanceModelMat, name);
        Entity::AddComponent(entity, MeshComp::GetInstance());
        MeshComp::AddInstance(instaceMeshes->at(i), entity, instanceModelMat);
    }
}
}  // namespace system
}  // namespace neith
