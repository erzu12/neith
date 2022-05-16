#include "sysPhysics.h"

#include "physics/physics.h"
#include "sysTransform.h"
#include <glm/gtx/string_cast.hpp>

#include "log.h"

namespace neith {
namespace system {

int AddRigidBody(unsigned int entityID, Collider *collider, float mass)
{
    glm::mat4 transform = GetGlobalModelMat(entityID);
    NT_INTER_INFO(glm::to_string(transform));
    return Physics::AddRigidBody(transform, collider, entityID, mass);
}
}  // namespace system
}  // namespace neith
