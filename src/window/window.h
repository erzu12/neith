#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>

namespace neith {
class Window {
private:
    static bool mResize;
    static int mWidth;
    static int mHeight;
    static GLFWwindow *mGLTFwindow;

public:
    Window();
    static void UpdateWindow();

    static int GetWidth();
    static int GetHeight();

    static void SetWidth(int width);
    static void SetHeight(int height);

    static void SetResize(bool resize);
    static bool GetResize();

    static GLFWwindow *GetGLFWwindow();
};

struct CallbackContext {
    bool firstMouse;
    float lastX, lastY;
    Window *win;
};
}  // namespace neith
