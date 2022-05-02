#pragma once

#include <glm/matrix.hpp>

namespace neith {
namespace system {
void Transform(unsigned int entity, glm::mat4 &transform);
glm::mat4 GetGlobalModelMat(unsigned int entityID);
// Transform GetGlobalTransform(unsigned int entityID);
}  // namespace system
}  // namespace neith
