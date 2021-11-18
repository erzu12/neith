#pragma once
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "vecmath.h"

struct CameraData {
	float yaw, pitch;
	Float3 camerFront;
	Float3 cameraPos;
	Float3 moveVec;
};

struct CameraData *CameraInit();

void CameraGetViewMat(struct CameraData *cd, vec4 *dest);

void CameraMouseInput(struct CameraData *cd ,float offestX, float offestY);

void CameraKeyInput(struct CameraData *cd, GLFWwindow *window);
