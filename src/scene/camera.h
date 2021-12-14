#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "vecmath.h"

class Camera {
public:
	float yaw, pitch;
	Float3 camerFront;
	Float3 cameraPos;
	Float3 moveVec;

    Camera();

    void CameraGetViewMat(vec4 *dest);

    void CameraMouseInput(float offestX, float offestY);

    void CameraKeyInput(GLFWwindow *window);
};
