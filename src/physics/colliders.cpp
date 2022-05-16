#include "colliders.h"

namespace neith {
BoxCollider::BoxCollider(float width, float length, float height)
{
    mShape = new btBoxShape(btVector3(width, length, height));
}
btCollisionShape *BoxCollider::GetShape() { return mShape; }
SphereCollider::SphereCollider(float radius) { mShape = new btSphereShape(btScalar(radius)); }
btCollisionShape *SphereCollider::GetShape() { return mShape; }
}  // namespace neith
