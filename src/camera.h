#pragma once
#include "time.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include "vecmath.h"

struct CameraData {
	float yaw, pitch;
	Float3 camerFront;
	Float3 cameraPos;
	Float3 moveVec;
};


struct CameraData *CameraInit() { 
	struct CameraData *cd = (struct CameraData *)malloc(sizeof(struct CameraData));

    cd->pitch = 0.0f;
    cd->yaw = -PI * 0.5f;

    cd->camerFront = F3Zero();

	cd->camerFront.z = -1.0f;
	cd->cameraPos = F3Zero();
	cd->cameraPos.z = 3.0f;
    cd->moveVec = F3Zero();

	return cd;
}

void CameraGetViewMat(struct CameraData *cd, vec4 *dest) {

    //TODO: make seperat function for pos update
    Float3 cameraUp = {0.0f, 1.0f, 0.0f};

	const float speed = 2.5f * DeltaTime();

    cd->cameraPos = F3Add(cd->cameraPos, F3Scale(cd->camerFront, cd->moveVec.z * speed));
    Float3 cameraRight = F3Noramlize(F3Cross(cd->camerFront, cameraUp));
    cd->cameraPos = F3Add(cd->cameraPos, F3Scale(cameraRight, cd->moveVec.x * speed));
    
    cd->cameraPos = F3Add(cd->cameraPos, F3Init(0.0f, cd->moveVec.y * speed, 0.0f));
    F3Print(cd->cameraPos);
    F3Print(cd->camerFront);
    F3Print(cameraUp);
    
    printf("\n{");
    for(int i = 0; i < 4; i++) {
        printf("%f, %f, %f, %f\n", dest[i][0], dest[i][1], dest[i][2], dest[i][3]);
    }
    printf("}\n");
       
    Look(cd->cameraPos, cd->camerFront, cameraUp, dest); 
    
    //vec3 cameraUp = {0.0f, 1.0f, 0.0f};

    //const float speed = 2.5f * DeltaTime();
    //vec3 moveForward = GLM_VEC3_ZERO_INIT;
    //glm_vec3_scale(cd->camerFront, cd->moveVec[2] * speed, moveForward);
    //glm_vec3_add(cd->cameraPos, moveForward, cd->cameraPos);

    //vec3 moveRight = GLM_VEC3_ZERO_INIT;
    //vec3 cameraRight = GLM_VEC3_ZERO_INIT;
    //glm_vec3_cross(cd->camerFront, cameraUp, cameraRight);
    //glm_normalize(cameraRight);
    //glm_vec3_scale(cameraRight, cd->moveVec[0] * speed, moveRight);
    //glm_vec3_add(cd->cameraPos, moveRight, cd->cameraPos);

    //glm_vec3_add(cd->cameraPos, (vec3){0, cd->moveVec[1] * speed, 0} , cd->cameraPos);
    //vec3 cameraPos;
    //F3ToArr(cd->cameraPos, cameraPos);
    //vec3 camerFront;
    //F3ToArr(cd->camerFront, camerFront);
    //glm_look(cameraPos, camerFront, (vec3){0.0f, 1.0f, 0.0f}, dest);
    
}

void CameraMouseInput(struct CameraData *cd ,float offestX, float offestY) {
	const float sens = 0.001f;
	offestX *= sens;
	offestY *= sens;

	cd->yaw += offestX;
	cd->pitch -= offestY;

	if(cd->pitch > PI * 0.4f)
		cd->pitch = PI * 0.4f;
	if(cd->pitch < -PI * 0.4f)
		cd->pitch = -PI * 0.4f;
	
	Float3 dir = F3Zero();
	dir.x = cos(cd->yaw) * cos(cd->pitch);
	dir.y = sin(cd->pitch);
	dir.z = sin(cd->yaw) * cos(cd->pitch);
	dir = F3Noramlize(dir);
    cd->camerFront = dir;
}

void CameraKeyInput(struct CameraData *cd, GLFWwindow *window) {
    cd->moveVec = F3Zero();
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cd->moveVec.z += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cd->moveVec.z += -1.0f;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cd->moveVec.x += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cd->moveVec.x += -1.0f;
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cd->moveVec.y += 1.0f;
	if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		cd->moveVec.y += -1.0f;
	cd->moveVec = F3Noramlize(cd->moveVec);
}

