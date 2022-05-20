#include "physics.h"

#include <math.h>

#include <glm/gtx/string_cast.hpp>

#include "log.h"
#include "scene/systems/sysTransform.h"
#include "timer.h"

namespace neith {

btAlignedObjectArray<btCollisionShape*> Physics::mCollisionShapes;
btDefaultCollisionConfiguration* Physics::mCollisionConfig;
btCollisionDispatcher* Physics::mDispatcher;
btBroadphaseInterface* Physics::mOverlappingPairCache;
btSequentialImpulseConstraintSolver* Physics::mSolver;
btDiscreteDynamicsWorld* Physics::mDynamicsWorld;

Physics::Physics()
{
    mCollisionConfig = new btDefaultCollisionConfiguration();

    mDispatcher = new btCollisionDispatcher(mCollisionConfig);

    mOverlappingPairCache = new btDbvtBroadphase();

    mSolver = new btSequentialImpulseConstraintSolver;

    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfig);

    mDynamicsWorld->setGravity(btVector3(0, -10, 0));
}

/// Add a Rigid Body
int Physics::AddRigidBody(glm::mat4& transform, Collider* collider, unsigned int entityID, float mass)
{
    // create a dynamic rigidbody

    btCollisionShape* colShape = collider->GetShape();
    mCollisionShapes.push_back(colShape);

    // Create Dynamic Objects
    btTransform startTransform;
    startTransform.setFromOpenGLMatrix(&transform[0][0]);

    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass, localInertia);

    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active'
    // objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    body->setUserIndex(entityID);
    body->setRestitution(0.0);
    body->setContactStiffnessAndDamping(pow(2.0f, 0.5 * 127), pow(2.0f, 0.4 * 127));
    // NT_INTER_INFO("{}   {}", body->getLinearDamping(), body->getAngularDamping());
    // body->setDamping(0.3f, 0.5f);
    // NT_INTER_INFO("{}   {}", body->getContactDamping(), body->getContactStiffness());

    mDynamicsWorld->addRigidBody(body);

    return mDynamicsWorld->getNumCollisionObjects() - 1;
}

void Physics::SetLinearDamping(int rigidBodyID, float linearDamping)
{
    btRigidBody* body = btRigidBody::upcast(mDynamicsWorld->getCollisionObjectArray()[rigidBodyID]);
    body->setDamping(linearDamping, body->getAngularDamping());
}
void Physics::SetAngularDamping(int rigidBodyID, float angularDamping)
{
    btRigidBody* body = btRigidBody::upcast(mDynamicsWorld->getCollisionObjectArray()[rigidBodyID]);
    body->setDamping(body->getLinearDamping(), angularDamping);
}
void Physics::SetContactDamping(int rigidBodyID, float contactDamping)
{
    btRigidBody* body = btRigidBody::upcast(mDynamicsWorld->getCollisionObjectArray()[rigidBodyID]);
    body->setContactStiffnessAndDamping(body->getContactStiffness(), pow(2.0f, contactDamping * 127.0f));
}
void Physics::SetContactStiffness(int rigidBodyID, float contactStiffness)
{
    btRigidBody* body = btRigidBody::upcast(mDynamicsWorld->getCollisionObjectArray()[rigidBodyID]);
    body->setContactStiffnessAndDamping(pow(2.0f, contactStiffness * 127.0f), body->getContactDamping());
}
void Physics::SetRestitution(int rigidBodyID, float restitution)
{
    btRigidBody* body = btRigidBody::upcast(mDynamicsWorld->getCollisionObjectArray()[rigidBodyID]);
    body->setRestitution(restitution);
}

void Physics::UpdatePhysics()
{
    mDynamicsWorld->stepSimulation(Time::DeltaTime(), 1);

    auto bodies = mDynamicsWorld->getNonStaticRigidBodies();

    // NT_INTER_INFO(mDynamicsWorld->getDispatcher()->getNumManifolds());
    if (mDynamicsWorld->getDispatcher()->getNumManifolds() != 0)
        // NT_INTER_INFO(mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(0)->getBody0()->getUserIndex());

        for (int i = 0; i < bodies.size(); i++) {
            int entityID = bodies[i]->getUserIndex();
            btTransform trans;

            bodies[i]->getMotionState()->getWorldTransform(trans);

            glm::mat4 transform;
            trans.getOpenGLMatrix(&transform[0][0]);
            system::SetGlobalTransform(entityID, transform);
        }

    // print positions of all objects
    for (int j = mDynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) {
        btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;

        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = obj->getWorldTransform();
        }
        // printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()),
        // float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
}

int Physics::AddKinematicRigidBody(unsigned int entityID, glm::mat4& transform, Collider* collider)
{
    btCollisionShape* colShape = collider->GetShape();
    mCollisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setFromOpenGLMatrix(&transform[0][0]);

    btRigidBody* body = new btRigidBody(0.0f, nullptr, collider->GetShape());

    body->setUserIndex(entityID);

    body->forceActivationState(DISABLE_DEACTIVATION);
    body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);

    mDynamicsWorld->addRigidBody(body);

    return mDynamicsWorld->getNumCollisionObjects() - 1;
}

struct ContactResultCallback : public btCollisionWorld::ContactResultCallback {
    ContactPoint* mContactPoints;
    int mCapacity;
    int mContactCount;

    ContactResultCallback(ContactPoint* contactPoints, int capacity)
        : mContactPoints(contactPoints), mCapacity(capacity), mContactCount(0){};

    virtual btScalar addSingleResult(btManifoldPoint& mp,
                                     const btCollisionObjectWrapper* colObj0Wrap,
                                     int partId0,
                                     int index0,
                                     const btCollisionObjectWrapper* colObj1Wrap,
                                     int partId1,
                                     int index1)
    {
        if (mContactCount < mCapacity) {
            mContactPoints[mContactCount].worldNormal.x = mp.m_normalWorldOnB.getX();
            mContactPoints[mContactCount].worldNormal.y = mp.m_normalWorldOnB.getY();
            mContactPoints[mContactCount].worldNormal.z = mp.m_normalWorldOnB.getZ();

            mContactPoints[mContactCount].worldPos.x = mp.getPositionWorldOnA().getX();
            mContactPoints[mContactCount].worldPos.y = mp.getPositionWorldOnA().getY();
            mContactPoints[mContactCount].worldPos.z = mp.getPositionWorldOnA().getZ();

            mContactPoints[mContactCount].depth = mp.getDistance();
        }
        mContactCount++;
        return 1.f;
    }
};

int Physics::GetContacPoints(unsigned int colliderID, ContactPoint* contactPoints, int capacity)
{
    btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[colliderID];
    btManifoldPoint* manifoldPoint;
    ContactResultCallback cb(contactPoints, capacity);

    mDynamicsWorld->getCollisionWorld()->contactTest(obj, cb);

    return cb.mContactCount;
}

Physics::~Physics()
{
    for (int i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        mDynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // delete collision shapes
    for (int j = 0; j < mCollisionShapes.size(); j++) {
        btCollisionShape* shape = mCollisionShapes[j];
        mCollisionShapes[j] = 0;
        delete shape;
    }

    // delete dynamics world
    delete mDynamicsWorld;

    // delete solver
    delete mSolver;

    // delete broadphase
    delete mOverlappingPairCache;

    // delete dispatcher
    delete mDispatcher;

    delete mCollisionConfig;

    mCollisionShapes.clear();
}
}  // namespace neith
