#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>

namespace neith {
class Window {
private:
    bool resize;
    int width, height;
    GLFWwindow *window;

public:
    Window();
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
    bool firstMouse;
    float lastX, lastY;
    Window *win;
};
}  // namespace neith
