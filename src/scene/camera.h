#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include "vecmath.h"
namespace neith {
    class Camera {
    public:
        float yaw, pitch;
        Float3 camerFront;
        Float3 cameraPos;
        Float3 moveVec;

        Camera();

        void CameraGetViewMat(glm::mat4 &dest);

        void CameraMouseInput(float offestX, float offestY);

        void CameraKeyInput(GLFWwindow *window);
    };
}
