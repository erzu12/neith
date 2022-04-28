#pragma once
#include <neith.h>

#include <glm/mat4x4.hpp>

class Camera {
public:
    Camera();
    void UpdateCamera();

private:
    float mYaw, mPitch;
    glm::vec3 mCameraFront;
    glm::vec3 mCameraPos;
    unsigned int camera;

    glm::vec3 KeyInput();
};
