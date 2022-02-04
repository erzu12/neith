#include "sysMesh.h"
#include "scene/components/meshComp.h"
#include "scene/components/compManager.h"
#include "scene/scene.h"

namespace neith {
    namespace system {
        int AddStaticPrimitive(unsigned int entity,
                                                 glm::mat4 modelMat,
                                                 float *vertices,
                                                 int vertCount,
                                                 int *indices,
                                                 int indCount,
                                                 int material)
        {

            Scene::GetEntity(entity)->AddComponentByID(CompManager::GetCompID<MeshComp>());
            return MeshComp::AddStaticPrimitive(entity, modelMat, vertices, vertCount, indices, indCount, material);

        }
    }
}
