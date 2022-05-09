#pragma once

#include <list>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "scene/scene.h"
#include "window/window.h"

#include <glm/vec3.hpp>


namespace neith {
struct Line {
    glm::vec3 start;
    glm::vec3 end;
    glm::vec3 color;
    float width;
    int duration;
};

class LineRenderer {
public:
    static void InitLineRenderer();

    static void RenderLines();
    static void RednerGrid();

    static void AddLine(glm::vec3 startPoint, glm::vec3 endPoint, glm::vec3 color, float width, int duration);

private:
    static unsigned int mLineShader;
    static std::list<Line*> mLines;
};
}  // namespace neith
