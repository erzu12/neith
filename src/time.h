
#pragma once
#include <GLFW/glfw3.h>

struct Time{
	float deltaTime;
} time;

float DeltaTime() {
	return time.deltaTime; 
}

void FrameTime() {
	static float lastFrame;
	float currentTime = glfwGetTime();
	time.deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
}
