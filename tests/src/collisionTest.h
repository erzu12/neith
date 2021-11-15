#pragma once
#include <cu.h>
#include "../../src/collision.h"

TEST(sphereTriangelIntersectionTestNoCollision) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.0f, 2.0f, 0.0f), 
                                            1.0f);
    assertFalse(result);
}

TEST(sphereTriangelIntersectionTestNoCollisionPlane) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(1.5f, 0.0f, 0.0f), 
                                            0.4f);
    assertFalse(result);
}

TEST(sphereTriangelIntersectionTestCenterOverTri) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.2f, 1.9f, 0.2f), 
                                            2.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriEdge1) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(-0.2f, 1.8f, 0.2f), 
                                            2.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriEdge2) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.2f, 0.2f, -0.2f), 
                                            1.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriEdge3) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.7f, -0.2f, 0.7f), 
                                            1.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriVert1) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(-0.2f, 1.9f, -0.2f), 
                                            2.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriVert2) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(1.2f, 0.2f, 0.0f), 
                                            1.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestCenterNotOverTriVert3) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.0f, -0.2f, 1.2f), 
                                            1.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestTriZero) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.2f, 0.0f), 
                                            1.0f);
    assertTrue(result);
}

TEST(sphereTriangelIntersectionTestRadiusZero) {
    bool result = SphereTriangleIntersction(F3Init(0.0f, 0.0f, 0.0f),
                                            F3Init(1.0f, 0.0f, 0.0f),
                                            F3Init(0.0f, 0.0f, 1.0f),
                                            F3Init(0.2f, 0.0f, 0.2f), 
                                            0.0f);
    assertTrue(result);
}
TEST_SUITE(collisionTests) {
    TEST_ADD(sphereTriangelIntersectionTestNoCollision),
    TEST_ADD(sphereTriangelIntersectionTestNoCollisionPlane),
    TEST_ADD(sphereTriangelIntersectionTestCenterOverTri),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriEdge1),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriEdge2),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriEdge3),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriVert1),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriVert2),
    TEST_ADD(sphereTriangelIntersectionTestCenterNotOverTriVert3),
    TEST_ADD(sphereTriangelIntersectionTestTriZero),
    TEST_ADD(sphereTriangelIntersectionTestRadiusZero),
    TEST_SUITES_CLOSURE
};
