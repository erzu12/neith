#include "camera.h"

#include <stdlib.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "timer.h"
#include "vecmath.h"
namespace neith {
Camera::Camera()
{
    mPitch = 0.0f;
    mYaw = -PI * 0.5f;

    mCamerFront = F3Zero();

    mCamerFront.z = -1.0f;
    mCameraPos = F3Zero();
    mCameraPos.z = 3.0f;
    mCameraPos.y = 3.0f;
    mMoveVec = F3Zero();
}

void Camera::CameraGetViewMat(glm::mat4 &dest)
{
    // TODO: make seperat function for pos update
    Float3 cameraUp = {0.0f, 1.0f, 0.0f};

    const float speed = 20.0f * Time::DeltaTime();

    mCameraPos = F3Add(mCameraPos, F3Scale(mCamerFront, mMoveVec.z * speed));
    Float3 cameraRight = F3Noramlize(F3Cross(mCamerFront, cameraUp));
    mCameraPos = F3Add(mCameraPos, F3Scale(cameraRight, mMoveVec.x * speed));

    mCameraPos = F3Add(mCameraPos, F3Init(0.0f, mMoveVec.y * speed, 0.0f));

    Look(mCameraPos, mCamerFront, cameraUp, dest);
}

void Camera::CameraMouseInput(float offestX, float offestY)
{
    const float sens = 0.001f;
    offestX *= sens;
    offestY *= sens;

    mYaw += offestX;
    mPitch -= offestY;

    if (mPitch > PI * 0.4f)
        mPitch = PI * 0.4f;
    if (mPitch < -PI * 0.4f)
        mPitch = -PI * 0.4f;

    Float3 dir = F3Zero();
    dir.x = cos(mYaw) * cos(mPitch);
    dir.y = sin(mPitch);
    dir.z = sin(mYaw) * cos(mPitch);
    dir = F3Noramlize(dir);
    mCamerFront = dir;
}

void Camera::CameraKeyInput(GLFWwindow *window)
{
    mMoveVec = F3Zero();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mMoveVec.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mMoveVec.z += -1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mMoveVec.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mMoveVec.x += -1.0f;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mMoveVec.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        mMoveVec.y += -1.0f;
    mMoveVec = F3Noramlize(mMoveVec);
}
}  // namespace neith
