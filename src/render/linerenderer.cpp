#include "linerenderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "defaults.h"
#include "log.h"
#include "render/shaders.h"
#include "scene/components/cameraComp.h"

namespace neith {
unsigned int LineRenderer::mLineShader;
unsigned int LineRenderer::mGridShader;
std::list<Line *> LineRenderer::mLines;

unsigned int LineRenderer::mGridVAO;
unsigned int LineRenderer::mSubGridVAO;

bool LineRenderer::mDrawGrid = false;
bool LineRenderer::mDrawLines = false;

void LineRenderer::InitLineRenderer()
{
    mLineShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "line.vert", NTH_ASSET_DIR "line.frag").mShaderProgram;
    mGridShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "grid.vert", NTH_ASSET_DIR "grid.frag").mShaderProgram;
    InitGrid();
}

void LineRenderer::AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width, int duration)
{
    if (mDrawLines) {
        mLines.push_back(new Line{ start, end, color, width, duration });
    }
}

void LineRenderer::RenderLines()
{
    if (mDrawGrid) {
        RednerGrid();
    }
    if (!mDrawLines) {
        return;
    }
    unsigned int VAO;
    unsigned int VBO;

    glUseProgram(mLineShader);

    glm::mat4 MVP = glm::mat4(1.0f);
    glm::mat4 view = CameraComp::GetViewMat();
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f,
                                            (float)Window::GetWidth() / (float)Window::GetHeight(), 0.1f, 2000000.0f);
    MVP = projection * view;

    auto itr = mLines.begin();
    while (itr != mLines.end()) {
        Line *line = *itr;

        float vertices[] = { line->start.x, line->start.y, line->start.z, line->end.x, line->end.y, line->end.z };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUniformMatrix4fv(glGetUniformLocation(mLineShader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(mLineShader, "color"), 1, &line->color[0]);

        glBindVertexArray(VAO);
        glLineWidth(line->width);
        glDrawArrays(GL_LINES, 0, 2);

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        line->duration--;
        if (line->duration == 0) {
            itr = mLines.erase(itr);
            delete line;
        }
        else {
            itr++;
        }
    }
}

void LineRenderer::InitGrid()
{
    int gridSize = 1000;
    int subGridSize = 200;

    unsigned int gridVBO;
    unsigned int subGridVBO;

    int sGridSize = (subGridSize - subGridSize / 10);
    int x = 0;
    int i = 0;
    float subGridVert[sGridSize * 12];
    while (x < subGridSize) {
        if (x % 10 == 0) {
            x++;
            continue;
        }
        subGridVert[i * 6] = x - subGridSize / 2;
        subGridVert[i * 6 + 1] = 0.0f;
        subGridVert[i * 6 + 2] = subGridSize / 2;

        subGridVert[i * 6 + 3] = x - subGridSize / 2;
        subGridVert[i * 6 + 4] = 0.0f;
        subGridVert[i * 6 + 5] = -subGridSize / 2;

        subGridVert[sGridSize * 6 + i * 6] = subGridSize / 2;
        subGridVert[sGridSize * 6 + i * 6 + 1] = 0.0f;
        subGridVert[sGridSize * 6 + i * 6 + 2] = x - subGridSize / 2;

        subGridVert[sGridSize * 6 + i * 6 + 3] = -subGridSize / 2;
        subGridVert[sGridSize * 6 + i * 6 + 4] = 0.0f;
        subGridVert[sGridSize * 6 + i * 6 + 5] = x - subGridSize / 2;
        i++;
        x++;
    }
    int lineCount = gridSize / 10;
    float gridVert[lineCount * 12];
    for (int x = 0; x < lineCount; x++) {
        gridVert[x * 6] = x * 10 - gridSize * 0.5f;
        gridVert[x * 6 + 1] = 0.0f;
        gridVert[x * 6 + 2] = gridSize * 0.5f;

        gridVert[x * 6 + 3] = x * 10 - gridSize * 0.5f;
        gridVert[x * 6 + 4] = 0.0f;
        gridVert[x * 6 + 5] = -gridSize * 0.5f;

        gridVert[lineCount * 6 + x * 6] = gridSize * 0.5f;
        gridVert[lineCount * 6 + x * 6 + 1] = 0.0f;
        gridVert[lineCount * 6 + x * 6 + 2] = x * 10 - gridSize * 0.5f;

        gridVert[lineCount * 6 + x * 6 + 3] = -gridSize * 0.5f;
        gridVert[lineCount * 6 + x * 6 + 4] = 0.0f;
        gridVert[lineCount * 6 + x * 6 + 5] = x * 10 - gridSize * 0.5f;
    }

    glGenVertexArrays(1, &mSubGridVAO);
    glGenBuffers(1, &subGridVBO);
    glBindVertexArray(mSubGridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, subGridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(subGridVert), subGridVert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &subGridVBO);

    glGenVertexArrays(1, &mGridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(mGridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVert), gridVert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &gridVBO);
}

void LineRenderer::RednerGrid()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(mGridShader);

    glm::vec3 cameraPos = CameraComp::GetCameraPos();

    glm::mat4 VP = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = CameraComp::GetViewMat();
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f,
                                            (float)Window::GetWidth() / (float)Window::GetHeight(), 0.1f, 2000000.0f);
    glm::vec3 gridPos = glm::vec3(floor(cameraPos.x / 10) * 10, cameraPos.y * -0.01, floor(cameraPos.z / 10) * 10);
    model = glm::translate(model, gridPos);
    VP = projection * view;

    glUniformMatrix4fv(glGetUniformLocation(mGridShader, "VP"), 1, GL_FALSE, &VP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(mGridShader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniform3fv(glGetUniformLocation(mGridShader, "CameraPos"), 1, &cameraPos[0]);

    glm::vec3 color = glm::vec3(0.16, 0.16f, 0.16f);
    glUniform1f(glGetUniformLocation(mGridShader, "dist"), 100);
    glUniform3fv(glGetUniformLocation(mGridShader, "color"), 1, &color[0]);

    glBindVertexArray(mGridVAO);
    glLineWidth(1);
    glDrawArrays(GL_LINES, 0, 100 * 4);

    color = glm::vec3(0.13, 0.13f, 0.13f);
    glUniform3fv(glGetUniformLocation(mGridShader, "color"), 1, &color[0]);
    glUniform1f(glGetUniformLocation(mGridShader, "dist"), 30);

    glBindVertexArray(mSubGridVAO);
    glLineWidth(1);
    glDrawArrays(GL_LINES, 0, 200 * 4);

    glDisable(GL_BLEND);
}

void LineRenderer::DrawGrid(bool drawGrid) { mDrawGrid = drawGrid; }
void LineRenderer::DrawLines(bool drawLines) { mDrawLines = drawLines; }
}  // namespace neith
