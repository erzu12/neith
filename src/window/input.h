#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace neith {
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double posX, double posY);
}  // namespace neith
