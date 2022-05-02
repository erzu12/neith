#include "input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "window.h"

namespace neith {
glm::vec2 Input::mLastMousePos = glm::vec2();

void Input::processInput(GLFWwindow *window)
{
    // struct CallbackContext *callbackContext = (struct CallbackContext *)glfwGetWindowUserPointer(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

bool Input::GetKeyDown(Window *window, Key key)
{
    return glfwGetKey(window->GetGLFWwindow(), static_cast<int>(key)) == GLFW_PRESS;
}

glm::vec2 Input::GetDeltaMouse(Window *window)
{
    double xPos;
    double yPos;

    glfwGetCursorPos(window->GetGLFWwindow(), &xPos, &yPos);
    glm::vec2 mouseDelta = glm::vec2(xPos, yPos) - mLastMousePos;
    mLastMousePos = glm::vec2(xPos, yPos);
    return mouseDelta;
}
}  // namespace neith
