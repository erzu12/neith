#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
namespace neith {

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
unsigned int CreatFrameBuffer(int width, int height);
unsigned int CreatIntermediateFrameBuffer(int width, int height, unsigned int *screenTexturem, unsigned int *screenDepthMap);
unsigned int CreatDepthMapFrameBuffer(unsigned int *depthMap);
}  // namespace neith
