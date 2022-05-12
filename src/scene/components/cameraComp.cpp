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
unsigned int CameraComp::mEntityID = 0;

CameraComp::CameraComp() {}

void CameraComp::AttachCamera(unsigned int entityID) { mEntityID = entityID; }
}  // namespace neith
