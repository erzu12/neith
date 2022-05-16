#include "sysPhysics.h"

#include "physics/physics.h"
#include "sysTransform.h"

namespace neith {
namespace system {

int AddRigidBody(unsigned int entityID, Collider &collider)
{
    glm::mat4 transform = GetGlobalModelMat(entityID);
    return Physics::AddRigidBody(transform, collider);
}
}  // namespace system
}  // namespace neith
