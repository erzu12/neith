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
#include "timer.h"

namespace neith {

bool Window::mResize = false;
int Window::mWidth = 1800;
int Window::mHeight = 900;
GLFWwindow* Window::mGLTFwindow;

static void error_callback(int error, const char* description) { NT_INTER_ERROR(description); }
void APIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if(severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    std::string typeStr;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeStr = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeStr = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeStr = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeStr = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeStr = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeStr = "OTHER";
            break;
    }
    switch (severity){
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            NT_INTER_INFO("[opengl] [{}] {}", typeStr, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            NT_INTER_WARN("[opengl] [{}] {}", typeStr, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            NT_INTER_ERROR("[opengl] [{}] {}", typeStr, message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            NT_INTER_CRITICAL("[opengl] [{}] {}", typeStr, message);
            break;
    }
}

Window::Window()
{
    Log::Init();
    glfwSetErrorCallback(error_callback);

    // GLFW
    if (!glfwInit()) {
        NT_INTER_CRITICAL("faild to init GLFW");
        throw WindowCreationException("faild to init GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
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
    glfwSetWindowFocusCallback(window, Input::window_focus_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NT_INTER_CRITICAL("faild to load glad");
        throw WindowCreationException("faild to load glad");
    }
    else {
        NT_INTER_INFO("OpenGL loaded successfully with version: {}", glGetString(GL_VERSION));
    }
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglMessageCallback, nullptr);

    mGLTFwindow = window;
    struct CallbackContext* cbc = (struct CallbackContext*)malloc(sizeof(struct CallbackContext));
    cbc->lastX = 900;
    cbc->lastY = 500;
    cbc->firstMouse = true;
    cbc->win = this;
    glfwSetWindowUserPointer(window, cbc);

    glfwSetCursorPosCallback(window, Input::mouse_callback);

    Input::ActivateMouse();
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
