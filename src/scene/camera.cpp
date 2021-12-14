#include "camera.h"

#include "time.h"

#include <stdlib.h>
#include <cglm/cglm.h>
#include "vecmath.h"

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

void Camera::CameraGetViewMat(vec4 *dest) {

    //TODO: make seperat function for pos update
    Float3 cameraUp = {0.0f, 1.0f, 0.0f};

	const float speed = 2.5f * Time::DeltaTime();

    cameraPos = F3Add(cameraPos, F3Scale(camerFront, moveVec.z * speed));
    Float3 cameraRight = F3Noramlize(F3Cross(camerFront, cameraUp));
    cameraPos = F3Add(cameraPos, F3Scale(cameraRight, moveVec.x * speed));
    
    cameraPos = F3Add(cameraPos, F3Init(0.0f, moveVec.y * speed, 0.0f));
    /*F3Print(cd->cameraPos);*/
    /*F3Print(cd->camerFront);*/
    /*F3Print(cameraUp);*/
    
    /*printf("\n{");*/
    /*for(int i = 0; i < 4; i++) {*/
        /*printf("%f, %f, %f, %f\n", dest[i][0], dest[i][1], dest[i][2], dest[i][3]);*/
    /*}*/
    /*printf("}\n");*/
       
    Look(cameraPos, camerFront, cameraUp, dest); 
    
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

