#include "cameraComp.h"

#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "log.h"
#include "timer.h"
namespace neith {
glm::mat4 CameraComp::mTransform = glm::mat4(1.0f);

DebugInfo *CameraComp::GetDebugInfo(unsigned int entityID)
{
    DebugInfo *nameInfo = new DebugInfo("Camera Component");
    DebugInfo *debugInfo = new DebugInfo("Camera Data:");
    debugInfo->child.push_back(new DebugInfo("Transform: " + glm::to_string(mTransform)));
    nameInfo->child.push_back(debugInfo);
    return nameInfo;
}
}  // namespace neith
