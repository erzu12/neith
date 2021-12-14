#include "time.h"
#include <GLFW/glfw3.h>

float Time::DeltaTime() {
	return deltaTime; 
}

void Time::FrameTime() {
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
}