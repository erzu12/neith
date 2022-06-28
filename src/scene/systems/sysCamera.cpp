#include "sysCamera.h"

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
    Entity::AddComponent(entityID, CameraComp::GetIndices());
    CameraComp::AttachCamera(entityID);
}
}  // namespace system
}  // namespace neith
