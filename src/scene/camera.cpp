#include "camera.h"

#include "timer.h"

#include <stdlib.h>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>

#include "vecmath.h"
namespace neith {
    Camera::Camera() { 
        pitch = 0.0f;
        yaw = -PI * 0.5f;

        camerFront = F3Zero();

        camerFront.z = -1.0f;
        cameraPos = F3Zero();
        cameraPos.z = 3.0f;
        cameraPos.y = 3.0f;
        moveVec = F3Zero();
    }

    void Camera::CameraGetViewMat(glm::mat4 &dest) {

        //TODO: make seperat function for pos update
        Float3 cameraUp = {0.0f, 1.0f, 0.0f};

        const float speed = 5.0f * Time::DeltaTime();

        cameraPos = F3Add(cameraPos, F3Scale(camerFront, moveVec.z * speed));
        Float3 cameraRight = F3Noramlize(F3Cross(camerFront, cameraUp));
        cameraPos = F3Add(cameraPos, F3Scale(cameraRight, moveVec.x * speed));
        
        cameraPos = F3Add(cameraPos, F3Init(0.0f, moveVec.y * speed, 0.0f));
           
        Look(cameraPos, camerFront, cameraUp, dest); 
        
    }

    void Camera::CameraMouseInput(float offestX, float offestY) {
        const float sens = 0.001f;
        offestX *= sens;
        offestY *= sens;

        yaw += offestX;
        pitch -= offestY;

        if(pitch > PI * 0.4f)
            pitch = PI * 0.4f;
        if(pitch < -PI * 0.4f)
            pitch = -PI * 0.4f;
        
        Float3 dir = F3Zero();
        dir.x = cos(yaw) * cos(pitch);
        dir.y = sin(pitch);
        dir.z = sin(yaw) * cos(pitch);
        dir = F3Noramlize(dir);
        camerFront = dir;
    }

    void Camera::CameraKeyInput(GLFWwindow *window) {
        moveVec = F3Zero();
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            moveVec.z += 1.0f;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            moveVec.z += -1.0f;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            moveVec.x += 1.0f;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            moveVec.x += -1.0f;
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            moveVec.y += 1.0f;
        if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            moveVec.y += -1.0f;
        moveVec = F3Noramlize(moveVec);
    }
}
