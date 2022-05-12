#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

namespace neith {
class Camera {
public:
    float mYaw, mPitch;
    glm::vec3 mCameraFront;
    glm::vec3 mCameraPos;
    glm::vec3 mMoveVec;

    Camera();

    void CameraGetViewMat(glm::mat4 &dest);

    void CameraMouseInput(float offestX, float offestY);

    void CameraKeyInput(GLFWwindow *window);
};
}  // namespace neith
