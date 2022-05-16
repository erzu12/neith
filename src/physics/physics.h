#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/mat4x4.hpp>

#include "physics/colliders.h"

namespace neith {

class Physics {
public:
    Physics();
    static void UpdatePhysics();

    static int AddRigidBody(glm::mat4& transform, Collider& collider);
    ~Physics();

private:
    static btAlignedObjectArray<btCollisionShape*> mCollisionShapes;

    static btDefaultCollisionConfiguration* mCollisionConfig;

    static btCollisionDispatcher* mDispatcher;

    static btBroadphaseInterface* mOverlappingPairCache;

    static btSequentialImpulseConstraintSolver* mSolver;

    static btDiscreteDynamicsWorld* mDynamicsWorld;
};
}  // namespace neith
