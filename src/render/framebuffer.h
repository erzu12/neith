#pragma once
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int CreatFrameBuffer(int width, int height, unsigned int *texColorBuffer);
unsigned int CreatIntermediateFrameBuffer(int width, int height, unsigned int *screenTexture);