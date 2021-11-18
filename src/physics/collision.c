#include "collision.h"

#include <math.h>
#include <stdbool.h>
//#include <cglm/cglm.h>
#include "vecmath.h"

bool SphereTriangleIntersction(Float3 t0, Float3 t1, Float3 t2, Float3 sc, float r) {
    Float3 N = F3Noramlize(F3Cross(F3Subtraction(t1, t0), F3Subtraction(t2, t0)));
    float dist = F3Dot(F3Subtraction(sc, t0), N);
    
    if(dist < -r || dist > r) {
        return false; //No collison
    }

    Float3 pPoint = F3Subtraction(sc, F3Scale(N, dist));

    Float3 c0 = F3Cross(F3Subtraction(pPoint, t0), F3Subtraction(t1, t0));
    Float3 c1 = F3Cross(F3Subtraction(pPoint, t1), F3Subtraction(t2, t1));
    Float3 c2 = F3Cross(F3Subtraction(pPoint, t2), F3Subtraction(t0, t2));
    bool inside = F3Dot(c0, N) <= 0 && F3Dot(c1, N) <= 0 && F3Dot(c2, N) <= 0;

    float rsq = r*r;

    Float3 p0 = ClosestPointOnLineSegment(t0, t1, sc);
    Float3 v0 = F3Subtraction(sc, p0);
    float distsq0 = F3Dot(v0, v0);
    bool intersects = distsq0 < rsq;

    Float3 p1 = ClosestPointOnLineSegment(t1, t2, sc);
    Float3 v1 = F3Subtraction(sc, p1);
    float distsq1 = F3Dot(v1, v1);
    intersects |= distsq1 < rsq;

    Float3 p2 = ClosestPointOnLineSegment(t2, t0, sc);
    Float3 v2 = F3Subtraction(sc, p2);
    float distsq2 = F3Dot(v2, v2);
    intersects |= distsq2 < rsq;

    if(inside || intersects) {
        Float3 bestPoint = pPoint;
        Float3 intersectionVec;

        if(inside) {
            intersectionVec = F3Subtraction(sc, pPoint);
        } else {
            Float3 d = F3Subtraction(sc, p0);
            float bestDistsq = F3Dot(d, d);
            bestPoint = p0;
            intersectionVec = d;

            d = F3Subtraction(sc, p1);
            float distsq = F3Dot(d, d);
            if(distsq < bestDistsq) {
                distsq = bestDistsq;
                bestPoint = p1;
                intersectionVec = d;
            }

            d = F3Subtraction(sc, p2);
            distsq = F3Dot(d, d);
            if(distsq < bestDistsq) {
                distsq = bestDistsq;
                bestPoint = p2;
                intersectionVec = d;
            }
        }
        return true;
    }
    return false;
}

Float3 ClosestPointOnLineSegment(Float3 a, Float3 b, Float3 point) {
    Float3 ab = F3Subtraction(b, a);
    float t = F3Dot(F3Subtraction(point, a), ab) / F3Dot(ab, ab);
    return F3Add(a, F3Scale(ab, fmin(fmax(t, 0.0), 1.0)));
}
