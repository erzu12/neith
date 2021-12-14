#pragma once
#include <GLFW/glfw3.h>

class Time {
private:
    static float deltaTime;
    static float lastFrame;
public:
    static float DeltaTime();
    static void FrameTime();
};
