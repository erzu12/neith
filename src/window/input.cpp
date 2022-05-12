#include "input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "debug.h"
#include "log.h"
#include "window.h"

namespace neith {
glm::vec2 Input::mLastMousePos = glm::vec2(0.0f);

void Input::processInput(GLFWwindow *window)
{
    // struct CallbackContext *callbackContext = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    static bool debug = false;
    static bool reset = true;
    if (GetKeyDown(Key::F3)) {
        if (reset) {
            reset = false;
            debug = !debug;
            Debug::EnableLines(debug);
            Debug::EnableGrid(debug);
        }
    }
    else {
        reset = true;
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
    double xPos;
    double yPos;

    glfwGetCursorPos(Window::GetGLFWwindow(), &xPos, &yPos);
    glm::vec2 mouseDelta = glm::vec2(xPos, yPos) - mLastMousePos;
    mLastMousePos = glm::vec2(xPos, yPos);
    return mouseDelta;
}
}  // namespace neith
