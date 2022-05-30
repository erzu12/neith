#include "window.h"

#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "exceptions/windowCreationException.h"
#include "input.h"
#include "log.h"
#include "render/framebuffer.h"
#include "scene/scene.h"
#include "timer.h"

namespace neith {

bool Window::mResize = false;
int Window::mWidth = 1800;
int Window::mHeight = 900;
GLFWwindow* Window::mGLTFwindow;

static void error_callback(int error, const char* description) { NT_INTER_ERROR(description); }

void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

Window::Window()
{
    glfwSetErrorCallback(error_callback);
    Log::Init();

    // GLFW
    if (!glfwInit()) {
        NT_INTER_CRITICAL("faild to init GLFW");
        throw WindowCreationException("faild to init GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1800, 1000, "Neith", NULL, NULL);
    if (window == NULL) {
        NT_INTER_CRITICAL("faild to crate window");
        glfwTerminate();
        throw WindowCreationException("faild to crate window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NT_INTER_CRITICAL("faild to load glad");
        throw WindowCreationException("faild to load glad");
    }
    mGLTFwindow = window;
    struct CallbackContext* cbc = (struct CallbackContext*)malloc(sizeof(struct CallbackContext));
    cbc->lastX = 900;
    cbc->lastY = 500;
    cbc->firstMouse = true;
    cbc->win = this;
    glfwSetWindowUserPointer(window, cbc);

    glfwSetCursorPosCallback(window, Input::mouse_callback);
}

int Window::GetWidth() { return mWidth; }

int Window::GetHeight() { return mHeight; }

void Window::SetWidth(int width) { mWidth = width; }

void Window::SetHeight(int height) { mHeight = height; }

bool Window::GetResize() { return mResize; }

void Window::SetResize(bool resize) { mResize = resize; }

GLFWwindow* Window::GetGLFWwindow() { return mGLTFwindow; }

bool Window::ShouldClose() { return glfwWindowShouldClose(mGLTFwindow); }

void Window::UpdateWindow()
{
    Time::FrameTime();
    Input::processInput(mGLTFwindow);
    glfwPollEvents();
}
}  // namespace neith
