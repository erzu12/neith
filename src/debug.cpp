#include "debug.h"

#include "render/linerenderer.h"
#include "log.h"
#include <glm/gtx/string_cast.hpp>

namespace neith {
bool Debug::mDrawGrid;
bool Debug::mDrawAxis;

void Debug::DrawLine(glm::vec3 start, glm::vec3 end) {
    LineRenderer::AddLine(start, end, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1);
}
void Debug::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
    LineRenderer::AddLine(start, end, color, 1.0f, 1); 
}
void Debug::DrawLine(glm::vec3 start, glm::vec3 end, float width) {
    LineRenderer::AddLine(start, end, glm::vec3(1.0f, 0.0f, 0.0f), width, 1);
}
void Debug::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width) {
    NT_INTER_INFO(glm::to_string(color));
    LineRenderer::AddLine(start, end, color, width, 1);
}
void Debug::DrawRay(glm::vec3 start, glm::vec3 dir, float length) {
    DrawRay(start, dir, length, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}
void Debug::DrawRay(glm::vec3 start, glm::vec3 dir, float length, glm::vec3 color) {
    DrawRay(start, dir, length, color, 1.0f);
}
void Debug::DrawRay(glm::vec3 start, glm::vec3 dir, float length, float width) {
    DrawRay(start, dir, length, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
}
void Debug::DrawRay(glm::vec3 start, glm::vec3 dir, float length, glm::vec3 color, float width) {
    glm::vec3 end = start + glm::normalize(dir) * length;
    LineRenderer::AddLine(start, end, color, width, 1);
}
void Debug::DrawGrid(bool drawGrid) { mDrawGrid = drawGrid; }
void Debug::DrawAxis(bool drawAxis) { mDrawAxis = drawAxis; }
}
