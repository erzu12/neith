#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vecmath.h>
#include "scene/scene.h"

struct Window {
    bool resize;
    int width, height;
    GLFWwindow *window;
};

struct CallbackContext {
    Float3 moveVec;
    bool firstMouse;
    float lastX, lastY;
    struct CameraData *cd;
    struct Window *win;
};
struct Window* CreateWindow();
void AttachSceneToWindow(struct Scene *sc, struct Window *win);
void UpdateWindow(struct Window* win);
