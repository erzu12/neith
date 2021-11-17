#pragma once
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vecmath.h>

struct Window {
    bool resize;
    int width, height;
};

struct CallbackContext {
    struct Window *window;
    Float3 moveVec;
    bool firstMouse;
    float lastX, lastY;
    struct CameraData *cd;
};
struct Window* CreateWindow();
void UpdateWindow(struct Window* win);
