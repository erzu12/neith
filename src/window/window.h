#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vecmath.h>

#include "scene/scene.h"

namespace neith {
class Window {
private:
    bool resize;
    int width, height;
    GLFWwindow *window;

public:
    Window();
    void AttachSceneToWindow(struct Scene *sc);
    void UpdateWindow();

    int GetWidth();
    int GetHeight();

    void SetWidth(int width);
    void SetHeight(int height);

    void SetResize(bool resize);
    bool GetResize();

    GLFWwindow *GetGLFWwindow();
};

struct CallbackContext {
    Float3 moveVec;
    bool firstMouse;
    float lastX, lastY;
    Camera *cd;
    struct Window *win;
};
}  // namespace neith
