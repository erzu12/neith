#pragma once

#define PI 3.14159265f

typedef struct Float3 {
    float x;
    float y;
    float z;
} Float3;

void F3ToArr(Float3 a, float *dest);

Float3 F3Zero();

Float3 F3Init(float x, float y, float z);

Float3 F3Add(Float3 a, Float3 b);

Float3 F3Subtraction(Float3 a, Float3 b);

float F3Magnitude(Float3 a);

Float3 F3Noramlize(Float3 a);

Float3 F3Scale(Float3 a, float b);

float F3Dot(Float3 a, Float3 b);

Float3 F3Cross(Float3 a, Float3 b);

void F3Print(Float3 vec);

void Lookat(Float3 eye, Float3 center, Float3 up, float mat[4][4]);

void Look(Float3 eye, Float3 dir, Float3 up, float mat[4][4]);

void Perspective(float fovy,
                      float aspect,
                      float nearZ,
                      float farZ,
                      float mat[4][4]);
