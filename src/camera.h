#pragma once
#include "time.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cglm/cglm.h>

struct CameraData {
	float yaw, pitch;
	vec3 camerFront;
	vec3 cameraPos;
	vec3 moveVec;
};

struct CameraData *CameraInit() {
	struct CameraData *cd = (struct CameraData *)malloc(sizeof(struct CameraData));

	cd->pitch = 0.0f;
	cd->yaw = -90.0f;

	glm_vec3_zero(cd->camerFront);
	cd->camerFront[2] = -1.0f;
	glm_vec3_zero(cd->cameraPos);
	cd->cameraPos[2] = 3.0f;
	glm_vec3_zero(cd->moveVec);

	return cd;
}

void CameraGetViewMat(struct CameraData *cd, vec4 *dest) {
	vec3 cameraUp = {0.0f, 1.0f, 0.0f};

	const float speed = 2.5f * DeltaTime();
	vec3 moveForward = GLM_VEC3_ZERO_INIT;
	glm_vec3_scale(cd->camerFront, cd->moveVec[2] * speed, moveForward);
	glm_vec3_add(cd->cameraPos, moveForward, cd->cameraPos);

	vec3 moveRight = GLM_VEC3_ZERO_INIT;
	vec3 cameraRight = GLM_VEC3_ZERO_INIT;
	glm_vec3_cross(cd->camerFront, cameraUp, cameraRight);
	glm_normalize(cameraRight);
	glm_vec3_scale(cameraRight, cd->moveVec[0] * speed, moveRight);
	glm_vec3_add(cd->cameraPos, moveRight, cd->cameraPos);

	glm_vec3_add(cd->cameraPos, (vec3){0, cd->moveVec[1] * speed, 0} , cd->cameraPos);
	glm_look(cd->cameraPos, cd->camerFront, cameraUp, dest);
}

void CameraMouseInput(struct CameraData *cd ,float offestX, float offestY) {
	const float sens = 0.06f;
	offestX *= sens;
	offestY *= sens;

	cd->yaw += offestX;
	cd->pitch -= offestY;

	if(cd->pitch > 89.0f)
		cd->pitch = 89.0f;
	if(cd->pitch < -89.0f)
		cd->pitch = -89.0f;
	
	vec3 dir = GLM_VEC3_ZERO_INIT;
	dir[0] = cos(glm_rad(cd->yaw)) * cos(glm_rad(cd->pitch));
	dir[1] = sin(glm_rad(cd->pitch));
	dir[2] = sin(glm_rad(cd->yaw)) * cos(glm_rad(cd->pitch));
	glm_normalize(dir);
	glm_vec3_copy(dir, cd->camerFront);

}

void CameraKeyInput(struct CameraData *cd, GLFWwindow *window) {
	glm_vec3_zero(cd->moveVec);
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cd->moveVec[2] += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cd->moveVec[2] += -1.0f;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cd->moveVec[0] += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cd->moveVec[0] += -1.0f;
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cd->moveVec[1] += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cd->moveVec[1] += -1.0f;
	glm_vec3_norm(cd->moveVec);
}

