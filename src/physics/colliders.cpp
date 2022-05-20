#include "colliders.h"

namespace neith {
BoxCollider::BoxCollider(float width, float length, float height)
{
    mShape = new btBoxShape(btVector3(width * 0.5f, length * 0.5f, height * 0.5f));
}
btCollisionShape *BoxCollider::GetShape() { return mShape; }
SphereCollider::SphereCollider(float radius) { mShape = new btSphereShape(btScalar(radius)); }
btCollisionShape *SphereCollider::GetShape() { return mShape; }
}  // namespace neith
