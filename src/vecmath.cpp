#include "vecmath.h"

// typedef float[3] float3;
#include <math.h>
#include <memory.h>
#include <stdio.h>

namespace neith {
void F3ToArr(Float3 a, float *dest)
{
    dest[0] = a.x;
    dest[1] = a.y;
    dest[2] = a.z;
}

Float3 F3Zero()
{
    Float3 a = { 0.0f, 0.0f, 0.0f };
    return a;
}

Float3 F3Init(float x, float y, float z)
{
    Float3 a = { x, y, z };
    return a;
}

Float3 F3Add(Float3 a, Float3 b)
{
    Float3 c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    c.z = a.z + b.z;
    return c;
}

Float3 F3Subtraction(Float3 a, Float3 b)
{
    Float3 c;
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    c.z = a.z - b.z;
    return c;
}

float F3Magnitude(Float3 a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }

Float3 F3Noramlize(Float3 a)
{
    float mag = F3Magnitude(a);
    if (mag == 0)
        return a;
    a.x /= mag;
    a.y /= mag;
    a.z /= mag;
    return a;
}

Float3 F3Scale(Float3 a, float b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

float F3Dot(Float3 a, Float3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Float3 F3Cross(Float3 a, Float3 b)
{
    Float3 c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}

void F3Print(Float3 vec)
{
    printf("{%f ,", vec.x);
    printf("%f ,", vec.y);
    printf("%f}\n", vec.z);
}

void Lookat(Float3 eye, Float3 center, Float3 up, glm::mat4 &mat)
{
    Float3 f = F3Subtraction(center, eye);
    f = F3Noramlize(f);

    Float3 s = F3Noramlize(F3Cross(f, up));
    Float3 u = F3Cross(s, f);

    mat[0][0] = s.x;
    mat[0][1] = u.x;
    mat[0][2] = -f.x;
    mat[1][0] = s.y;
    mat[1][1] = u.y;
    mat[1][2] = -f.y;
    mat[2][0] = s.z;
    mat[2][1] = u.z;
    mat[2][2] = -f.z;
    mat[3][0] = -F3Dot(s, eye);
    mat[3][1] = -F3Dot(u, eye);
    mat[3][2] = F3Dot(f, eye);
    mat[0][3] = mat[1][3] = mat[2][3] = 0.0f;
    mat[3][3] = 1.0f;
}

void Look(Float3 eye, Float3 dir, Float3 up, glm::mat4 &mat)
{
    Float3 target = F3Add(eye, dir);
    Lookat(eye, target, up, mat);
}

void Perspective(float fovy, float aspect, float nearZ, float farZ, float mat[4][4])
{
    float f, fn;

    memset(mat, 0, sizeof(float) * 16);

    f = 1.0f / tanf(fovy * 0.5f);
    fn = 1.0f / (nearZ - farZ);

    mat[0][0] = f / aspect;
    mat[1][1] = f;
    mat[2][2] = (nearZ + farZ) * fn;
    mat[2][3] = -1.0f;
    mat[3][2] = 2.0f * nearZ * farZ * fn;
}
}  // namespace neith
