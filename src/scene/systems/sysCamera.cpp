#include "sysCamera.h"

#include "scene/components/compManager.h"
#include "sysTransform.h"

namespace neith {
namespace system {
// glm::mat4 GetViewMat()
//{
// unsigned int entityID = CameraComp::GetEntityID();
// return glm::inverse(system::GetGlobalModelMat(entityID));
//}

void AddCamera(unsigned int entityID)
{
    Scene::GetEntity(entityID)->AddComponentByID(CompManager::GetCompID<CameraComp>());
    CameraComp::AttachCamera(entityID);
}
}  // namespace system
}  // namespace neith
