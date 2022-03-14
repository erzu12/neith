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
        void window_focus_callback(GLFWwindow* window, int focused);
        void UpdateWindow();
    };

    struct CallbackContext {
        Float3 moveVec;
        bool firstMouse;
        float lastX, lastY;
        Camera *cd;
        struct Window *win;
    };
}
