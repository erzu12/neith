#include "window.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "vecmath.h"
#include "input.h"


struct Window *CreateWindow() {
    //GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1800, 1000, "Neith", NULL, NULL);
    if(window == NULL) {
        printf("faild to crate window\n");
        glfwTerminate();
        exit( -1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return -1;
        }    
    struct Window win;
    win.resize = false;
    win.width = 1800;
    win.height = 900; 
    struct CallbackContext cbc;
    cbc.lastX = 900;
    cbc.lastY = 500;
    cbc.firstMouse = true;
    cbc.window = win;
    glfwSetWindowUserPointer(window, &cbc);

    glfwSetCursorPosCallback(window, mouse_callback);
}

void UpdateWindow(GLFWwindow* window) {
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void DeleteWindow(GLFWwindow* window) {
}
