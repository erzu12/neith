#pragma once

#include <btBulletDynamicsCommon.h>

namespace neith {
class Collider {
public:
    virtual btCollisionShape *GetShape() = 0;
};

class BoxCollider : public Collider {
public:
    BoxCollider(float width, float length, float hight);
    btCollisionShape *GetShape();

private:
    btBoxShape *mShape;
};
class SphereCollider {
public:
    SphereCollider(float radius);
    btCollisionShape *GetShape();

private:
    btSphereShape *mShape;
};
}  // namespace neith
