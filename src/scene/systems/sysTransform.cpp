#include "sysTransform.h"

#include "scene/components/compManager.h"
#include "scene/components/meshComp.h"
#include "scene/components/transformComp.h"
#include "scene/scene.h"

namespace neith {
namespace system {

void UpdateChildren(Entity *entity, glm::mat4 &transform)
{
    if (entity->HasComponentByID(CompManager::GetCompID<MeshComp>())) {
        MeshComp::Transform(entity->GetID(), transform);
    }
    for (int i = 0; entity->GetChildCount() > i; i++) {
        UpdateChildren(entity->GetChild(i), transform);
    }
}

void Transform(unsigned int entityID, glm::mat4 &transform)
{
    Entity *entity = Scene::GetEntity(entityID);
    TransformComp::Transforme(entityID, transform);
    UpdateChildren(entity, transform);
}
}  // namespace system
}  // namespace neith
