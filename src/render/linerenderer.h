#pragma once

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <list>

#include "scene/scene.h"
#include "window/window.h"

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

    static void DrawGrid(bool drawGrid);
    static void DrawLines(bool drawLines);
    static void AddLine(glm::vec3 startPoint, glm::vec3 endPoint, glm::vec3 color, float width, int duration);

private:
    static void InitGrid();
    static void RednerGrid();

    static unsigned int mLineShader;
    static unsigned int mGridShader;
    static std::list<Line*> mLines;

    static unsigned int mGridVAO;
    static unsigned int mSubGridVAO;

    static bool mDrawGrid;
    static bool mDrawLines;
};
}  // namespace neith
