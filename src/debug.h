#pragma once
#include <glm/vec3.hpp>

namespace neith {

class Debug {
public:
    static void DrawLine(glm::vec3 start, glm::vec3 end);
    static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
    static void DrawLine(glm::vec3 start, glm::vec3 end, float width);
    static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width);

    static void DrawRay(glm::vec3 start, glm::vec3 dir, float length);
    static void DrawRay(glm::vec3 start, glm::vec3 dir, float length, glm::vec3 color);
    static void DrawRay(glm::vec3 start, glm::vec3 dir, float length, float width);
    static void DrawRay(glm::vec3 start, glm::vec3 dir, float length, glm::vec3 color, float width);

    static void EnableGrid(bool enableGrid);
    static void EnableLines(bool enableLines);

private:
};

namespace Color {
static const glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
static const glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
static const glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
static const glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);
static const glm::vec3 magenta = glm::vec3(1.0f, 0.0f, 1.0f);
static const glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
static const glm::vec3 gray = glm::vec3(0.5f, 0.5f, 0.5f);
static const glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
}  // namespace Color

}  // namespace neith
