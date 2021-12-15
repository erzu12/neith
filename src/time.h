#pragma once

class Time {
private:
    static float deltaTime;
    static float lastFrame;
public:
    static float DeltaTime();
    static void FrameTime();
};
