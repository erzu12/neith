#include "sysTransform.h"

#include "scene/components/cameraComp.h"
#include "scene/components/meshComp.h"
#include "scene/components/transformComp.h"
#include "scene/scene.h"

#include <glm/gtx/string_cast.hpp>
#include "log.h"

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
        unsigned int chieldID = Entity::GetChild(entityID, i);
        UpdateChildren(chieldID, transform);
        TransformComp::TransformeGlobal(chieldID, transform);
    }
}

void Transform(unsigned int entityID, glm::mat4 &transform)
{
    TransformComp::Transforme(entityID, transform);
    TransformComp::TransformeGlobal(entityID, transform);
    UpdateChildren(entityID, transform);
}

void SetGlobalTransform(unsigned int entityID, glm::mat4 &transform) {
    glm::mat4 newTransform = glm::inverse(GetGlobalModelMat(entityID)) * transform;
    Transform(entityID, newTransform);
}
glm::mat4 GetGlobalModelMat(unsigned int entityID)
{
    return TransformComp::GetGlobalTransform(entityID);
}
}  // namespace system
}  // namespace neith
