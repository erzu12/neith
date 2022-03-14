#include "window.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <iostream>

#include "log.h"
#include "timer.h"
#include "scene/scene.h"
#include "vecmath.h"
#include "input.h"
#include "render/framebuffer.h"

namespace neith {
    static void error_callback(int error, const char* description) {
        NT_INTER_ERROR(description);
    }

    Window::Window() {
        glfwSetErrorCallback(error_callback);

        //GLFW
        if(!glfwInit()) {
            NT_INTER_CRITICAL("faild to init GLFW");
            return nullptr;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(1800, 1000, "Neith", NULL, NULL);
        if(window == NULL) {
            NT_INTER_CRITICAL("faild to crate window");
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                exit(-1);
            }    
        struct Window *win = (struct Window*)malloc(sizeof(struct Window));
        win->resize = false;
        win->width = 1800;
        win->height = 900; 
        win->window = window;
        struct CallbackContext *cbc = (struct CallbackContext*)malloc(sizeof(struct CallbackContext));
        cbc->lastX = 900;
        cbc->lastY = 500;
        cbc->firstMouse = true;
        cbc->win = win;
        glfwSetWindowUserPointer(window, cbc);

        glfwSetCursorPosCallback(window, mouse_callback);

        return win;
    }

    void AttachSceneToWindow(struct Scene *sc, struct Window *win) {
        struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(win->window);
        cbc->cd = sc->cd;
    }

    void UpdateWindow(struct Window *win) {
        Time::FrameTime();
        processInput(win->window);
        glfwPollEvents();
    }

    void window_focus_callback(GLFWwindow* window, int focused) {
        if (focused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    void DeleteWindow(GLFWwindow* window) {
    }
}
