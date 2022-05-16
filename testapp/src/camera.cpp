#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#define PI glm::pi<float>()

Camera::Camera()
{
    mPitch = 0.0f;
    mYaw = -PI * 0.5f;

    mCameraPos = glm::vec3(0.0f);
    mCameraPos.z = 10.0f;
    mCameraPos.y = 5.0f;

    camera = neith::AddEntity("camera");
    neith::AttachCamera(camera);
}

void Camera::UpdateCamera()
{
    const float speed = 10.0f * neith::Time::DeltaTime();
    const float sens = 0.0005f;

    glm::vec2 deltaMouse = neith::GetDeltaMouse();

    // NT_INFO("{}, {}", deltaMouse.x, deltaMouse.y);

    deltaMouse *= sens;

    mYaw += deltaMouse.x;
    mPitch -= deltaMouse.y;

    if (mPitch > PI * 0.4f)
        mPitch = PI * 0.4f;
    if (mPitch < -PI * 0.4f)
        mPitch = -PI * 0.4f;

    glm::vec3 cameraFront = glm::vec3();
    cameraFront.x = sin(mYaw) * cos(mPitch);
    cameraFront.y = sin(mPitch);
    cameraFront.z = -cos(mYaw) * cos(mPitch);
    // NT_INFO(glm::to_string(cameraFront));
    if (glm::length(cameraFront) != 0) {
        cameraFront = glm::normalize(cameraFront);
    }

    glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

    glm::vec3 moveVec = KeyInput();

    mCameraPos = mCameraPos + cameraFront * (moveVec.z * speed);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    mCameraPos = mCameraPos + cameraRight * (moveVec.x * speed);

    mCameraPos = mCameraPos + glm::vec3(0.0f, moveVec.y * speed, 0.0f);

    // glm::vec3 target = mCameraPos + cameraFront;
    // glm::mat4 transform = glm::lookAt(mCameraPos, target, cameraUp);
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, mCameraPos);
    transform = glm::rotate(transform, -mYaw, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, mPitch, glm::vec3(1.0f, 0.0f, 0.0f));
    // NT_INFO(glm::to_string(transform));
    neith::TransformEntity(camera, transform);
}

glm::vec3 Camera::KeyInput()
{
    glm::vec3 moveVec = glm::vec3(0.0f);
    if (neith::GetKey(neith::Key::W))
        moveVec.z += 1.0f;
    if (neith::GetKey(neith::Key::S))
        moveVec.z += -1.0f;
    if (neith::GetKey(neith::Key::D))
        moveVec.x += 1.0f;
    if (neith::GetKey(neith::Key::A))
        moveVec.x += -1.0f;
    if (neith::GetKey(neith::Key::Space))
        moveVec.y += 1.0f;
    if (neith::GetKey(neith::Key::Left_Control))
        moveVec.y += -1.0f;
    if (glm::length(moveVec) != 0) {
        moveVec = glm::normalize(moveVec);
    }
    return moveVec;
}
