#include "input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtx/string_cast.hpp>

#include "debug.h"
#include "gui/gui.h"
#include "log.h"
#include "window.h"

namespace neith {
glm::vec2 Input::mLastMousePos = glm::vec2(0.0f);
bool Input::mMouseActive = true;

void Input::processInput(GLFWwindow *window)
{
    // struct CallbackContext *callbackContext = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    static bool debug = false;
    static bool resetDebug = true;
    if (GetKeyDown(Key::F3)) {
        if (resetDebug) {
            resetDebug = false;
            debug = !debug;
            Debug::EnableLines(debug);
            Debug::EnableGrid(debug);
        }
    }
    else {
        resetDebug = true;
    }
    static bool gui = false;
    static bool resetGui = true;
    if (GetKeyDown(Key::F4)) {
        if (resetGui) {
            resetGui = false;
            gui = !gui;
            Gui::ShouldRenderGui(gui);
            if (gui) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                DeactivateMouse();
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                ActivateMouse();
            }
        }
    }
    else {
        resetGui = true;
    }
}

void Input::mouse_callback(GLFWwindow *window, double posX, double posY)
{
    struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if (cbc->firstMouse) {
        cbc->lastX = posX;
        cbc->lastY = posY;
        cbc->firstMouse = false;
    }

    cbc->lastX = posX;
    cbc->lastY = posY;
}

bool Input::GetKeyDown(Key key) { return glfwGetKey(Window::GetGLFWwindow(), static_cast<int>(key)) == GLFW_PRESS; }

glm::vec2 Input::GetDeltaMouse()
{
    if (!mMouseActive) {
        return glm::vec2(0.0f);
    }
    double xPos;
    double yPos;

    glfwGetCursorPos(Window::GetGLFWwindow(), &xPos, &yPos);
    glm::vec2 mouseDelta = glm::vec2(xPos, yPos) - mLastMousePos;
    mLastMousePos = glm::vec2(xPos, yPos);
    return mouseDelta;
}
void Input::ActivateMouse()
{
    mMouseActive = true;

    double xPos;
    double yPos;

    glfwGetCursorPos(Window::GetGLFWwindow(), &xPos, &yPos);
    mLastMousePos = glm::vec2(xPos, yPos);
}
void Input::DeactivateMouse() { mMouseActive = false; }

void Input::window_focus_callback(GLFWwindow *window, int focused)
{
    // if (focused) {
    // if (mMouseActive) {
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //}
    //}
}
}  // namespace neith
