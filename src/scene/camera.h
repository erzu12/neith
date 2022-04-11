#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

#include "vecmath.h"
namespace neith {
class Camera {
public:
    float mYaw, mPitch;
    Float3 mCamerFront;
    Float3 mCameraPos;
    Float3 mMoveVec;

    Camera();

    void CameraGetViewMat(glm::mat4 &dest);

    void CameraMouseInput(float offestX, float offestY);

    void CameraKeyInput(GLFWwindow *window);
};
}  // namespace neith
