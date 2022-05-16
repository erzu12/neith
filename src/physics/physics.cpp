#include "physics.h"

#include "log.h"
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
    NT_INTER_INFO("bullet");
    mCollisionConfig = new btDefaultCollisionConfiguration();

    mDispatcher = new btCollisionDispatcher(mCollisionConfig);

    mOverlappingPairCache = new btDbvtBroadphase();

    mSolver = new btSequentialImpulseConstraintSolver;

    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfig);

    mDynamicsWorld->setGravity(btVector3(0, -10, 0));

    /// create a few basic rigid bodies

    // the ground is a cube of side 100 at position y = -56.
    // the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        mCollisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        // rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        // using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        // add the body to the dynamics world
        mDynamicsWorld->addRigidBody(body);
    }

    {
        // create a dynamic rigidbody

        // btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        mCollisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        // rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(2, 10, 0));

        // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active'
        // objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        mDynamicsWorld->addRigidBody(body);
    }
}

int Physics::AddRigidBody(glm::mat4& transform, Collider& collider)
{
    // create a dynamic rigidbody

    // btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    btCollisionShape* colShape = collider.GetShape();
    mCollisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setFromOpenGLMatrix(&transform[0][0]);

    btScalar mass(1.f);

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

    mDynamicsWorld->addRigidBody(body);

    return mCollisionShapes.size() - 1;
}

void Physics::UpdatePhysics()
{
    mDynamicsWorld->stepSimulation(Time::DeltaTime(), 10);

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
