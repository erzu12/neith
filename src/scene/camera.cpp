#include "camera.h"

#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "log.h"
#include "timer.h"
namespace neith {

Camera::Camera()
{
    mPitch = 0.0f;
    mYaw = -glm::pi<float>() * 0.5f;

    mCameraFront = glm::vec3(0.0f);

    mCameraFront.z = -1.0f;
    mCameraPos = glm::vec3(0.0f);
    mCameraPos.z = 3.0f;
    mCameraPos.y = 3.0f;
    mMoveVec = glm::vec3(0.0f);
}

void Camera::CameraGetViewMat(glm::mat4 &dest)
{
    // TODO: make seperat function for pos update
    glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

    const float speed = 5.0f * Time::DeltaTime();

    mCameraPos = mCameraPos + mCameraFront * (mMoveVec.z * speed);
    glm::vec3 cameraRight = glm::normalize(glm::cross(mCameraFront, cameraUp));
    mCameraPos = mCameraPos + cameraRight * (mMoveVec.x * speed);

    mCameraPos = mCameraPos + glm::vec3(0.0f, mMoveVec.y * speed, 0.0f);

    glm::vec3 target = mCameraPos + mCameraFront;
    dest = glm::lookAt(mCameraPos, target, cameraUp);
    // NT_INTER_INFO(glm::to_string(dest));
}

void Camera::CameraMouseInput(float offestX, float offestY)
{
    const float sens = 0.0005f;
    offestX *= sens;
    offestY *= sens;

    mYaw += offestX;
    mPitch -= offestY;

    if (mPitch > glm::pi<float>() * 0.4f)
        mPitch = glm::pi<float>() * 0.4f;
    if (mPitch < -glm::pi<float>() * 0.4f)
        mPitch = -glm::pi<float>() * 0.4f;

    glm::vec3 dir = glm::vec3();
    dir.x = cos(mYaw) * cos(mPitch);
    dir.y = sin(mPitch);
    dir.z = sin(mYaw) * cos(mPitch);
    if (glm::length(mMoveVec) != 0) {
        dir = glm::normalize(dir);
    }
    mCameraFront = dir;
}

void Camera::CameraKeyInput(GLFWwindow *window)
{
    mMoveVec = glm::vec3(0.0f);
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
    if (glm::length(mMoveVec) != 0) {
        mMoveVec = glm::normalize(mMoveVec);
    }
}
}  // namespace neith
