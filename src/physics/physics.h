#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/mat4x4.hpp>

#include "physics/colliders.h"

namespace neith {
struct ContactPoint {
    ContactPoint() : worldNormal(glm::vec3(0.0f)), worldPos(glm::vec3(0.0f)), depth(0) {}
    glm::vec3 worldNormal;
    glm::vec3 worldPos;
    float depth;
};

class Physics {
public:
    Physics();
    static void UpdatePhysics();

    static int AddRigidBody(glm::mat4& transform, Collider* collider, unsigned int entityID, float mass);

    static void SetLinearDamping(int rigidBodyID, float linearDamping);
    static void SetAngularDamping(int rigidBodyID, float angularDamping);
    static void SetContactDamping(int rigidBodyID, float contactDamping);
    static void SetContactStiffness(int rigidBodyID, float contactStiffness);
    static void SetRestitution(int rigidBodyID, float restitution);

    static int AddKinematicRigidBody(unsigned int entityID, glm::mat4& transform, Collider* collider);
    static int AddCollider(unsigned int entityID, glm::mat4& transform, Collider* collider);
    static int GetContacPoints(unsigned int colliderID, ContactPoint* contactPoints, int capacity);

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
