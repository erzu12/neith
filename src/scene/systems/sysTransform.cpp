#include "sysTransform.h"

#include "scene/components/cameraComp.h"
#include "scene/components/meshComp.h"
#include "scene/components/transformComp.h"
#include "scene/scene.h"

namespace neith {
namespace system {

void UpdateChildren(unsigned int entityID, glm::mat4 &transform)
{
    if (Entity::HasComponent(entityID, "Mesh")) {
        MeshComp::Transform(entityID, transform);
    }
    if (Entity::HasComponent(entityID, "Camera")) {
        CameraComp::Transform(transform);
    }
    for (int i = 0; Entity::GetChildCount(entityID) > i; i++) {
        UpdateChildren(Entity::GetChild(entityID, i), transform);
    }
}

void Transform(unsigned int entityID, glm::mat4 &transform)
{
    TransformComp::Transforme(entityID, transform);
    UpdateChildren(entityID, transform);
}
glm::mat4 GetGlobalModelMat(unsigned int entityID)
{
    glm::mat4 transform = glm::mat4(1.0f);
    while (entityID != 0) {
        transform += TransformComp::GetTransform(entityID);
        entityID = Entity::GetParent(entityID);
    }
    return transform;
}
}  // namespace system
}  // namespace neith
