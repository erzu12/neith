#include "input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "scene/camera.h"

void processInput(GLFWwindow *window) {
    struct CallbackContext *callbackContext = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //glfwSetWindowShouldClose(window, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    CameraKeyInput(callbackContext->cd, window);
}

void mouse_callback(GLFWwindow *window, double posX, double posY){
    struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if(cbc->firstMouse) {
        cbc->lastX = posX;
        cbc->lastY = posY;
        cbc->firstMouse = false;
    }

    float offestX = posX - cbc->lastX;
    float offestY = posY - cbc->lastY;
    cbc->lastX = posX;
    cbc->lastY = posY;

    CameraMouseInput(cbc->cd, offestX, offestY);
}
