#pragma once

#include <stdbool.h>

#include "vecmath.h"

namespace neith {
bool SphereTriangleIntersction(Float3 t0, Float3 t1, Float3 t2, Float3 sc, float r);

Float3 ClosestPointOnLineSegment(Float3 a, Float3 b, Float3 point);
}  // namespace neith
