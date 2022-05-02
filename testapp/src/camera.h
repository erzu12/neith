#pragma once
#include <neith.h>

#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera(neith::Window* window);
    void UpdateCamera();

private:
    float mYaw, mPitch;
    glm::vec3 mCameraFront;
    glm::vec3 mCameraPos;
    neith::Window* mWindow;
    unsigned int camera;

    glm::vec3 KeyInput();
};
