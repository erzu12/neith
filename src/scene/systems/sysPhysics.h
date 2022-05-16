#pragma once

#include "physics/colliders.h"

namespace neith {
namespace system {
int AddRigidBody(unsigned int entityID, Collider *collider, float mass);
}
}  // namespace neith
