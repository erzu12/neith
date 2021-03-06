#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include "window.h"

namespace neith {
enum class Key {
    Unknown = -1,
    Space = 32,
    Apostrophe = 39, /* ' */
    Comma = 44,      /* , */
    Minus = 45,      /* - */
    Period = 46,     /* . */
    Slash = 47,      /* / */
    Num0 = 48,
    Num1 = 49,
    Num2 = 50,
    Num3 = 51,
    Num4 = 52,
    Num5 = 53,
    Num6 = 54,
    Num7 = 55,
    Num8 = 56,
    Num9 = 57,
    Semicolon = 59, /* ; */
    Equal = 61,     /* = */
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Left_Bracket = 91,  /* [ */
    Backslash = 92,     /* \ */
    Right_Bracket = 93, /* ] */
    Grave_Accent = 96,  /* ` */
    World_1 = 161,      /* non-US #1 */
    World_2 = 162,      /* non-US #2 */
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    Page_Up = 266,
    Page_Down = 267,
    Home = 268,
    End = 269,
    Caps_Lock = 280,
    Scroll_Lock = 281,
    Num_Lock = 282,
    Print_Screen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    NumPad_0 = 320,
    NumPad_1 = 321,
    NumPad_2 = 322,
    NumPad_3 = 323,
    NumPad_4 = 324,
    NumPad_5 = 325,
    NumPad_6 = 326,
    NumPad_7 = 327,
    NumPad_8 = 328,
    NumPad_9 = 329,
    NumPad_Decimal = 330,
    NumPad_Divide = 331,
    NumPad_Multiply = 332,
    NumPad_Subtract = 333,
    NumPad_Add = 334,
    NumPad_Enter = 335,
    NumPad_Equal = 336,
    Left_Shift = 340,
    Left_Control = 341,
    Left_Alt = 342,
    Left_Super = 343,
    Right_Shift = 344,
    Right_Control = 345,
    Right_Alt = 346,
    Right_Super = 347,
    Menu = 348
};
class Input {
public:
    static bool GetKeyDown(Key key);
    static glm::vec2 GetDeltaMouse();
    static void processInput(GLFWwindow *window);
    static void mouse_callback(GLFWwindow *window, double posX, double posY);
    static void ActivateMouse();
    static void window_focus_callback(GLFWwindow *window, int focused);

private:
    static void DeactivateMouse();
    static glm::vec2 mLastMousePos;
    static bool mMouseActive;
};
}  // namespace neith
