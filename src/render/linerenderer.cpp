#include "linerenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "defaults.h"
#include "scene/components/cameraComp.h"
#include "render/shaders.h"
#include "log.h"

namespace neith {
unsigned int LineRenderer::mLineShader;
std::list<Line*> LineRenderer::mLines;

void LineRenderer::InitLineRenderer() {
    mLineShader = LoadAndCompileShaders(NTH_ASSET_DIR "line.vert", NTH_ASSET_DIR "line.frag");
}

void LineRenderer::AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float width, int duration) {
    mLines.push_back(new Line{start, end, color, width, duration});
}

void LineRenderer::RenderLines() {

    unsigned int VAO;
    unsigned int VBO;

    glUseProgram(mLineShader);

    glm::mat4 MVP = glm::mat4(1.0f);
    glm::mat4 view = CameraComp::GetViewMat();
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f, (float)Window::GetWidth() / (float)Window::GetHeight(), 0.1f, 2000000.0f);
    MVP = projection * view * MVP;

    auto itr = mLines.begin();
    while(itr != mLines.end()) {

        Line *line = *itr;

        float vertices[] = {
            line->start.x, line->start.y, line->start.z,
            line->end.x, line->end.y, line->end.z
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);

        glUniformMatrix4fv(glGetUniformLocation(mLineShader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(mLineShader, "color"), 1, &line->color[0]);

        glBindVertexArray(VAO);
        glLineWidth(line->width);
        glDrawArrays(GL_LINES, 0, 2);
        line->duration--;
        if(line->duration == 0) {
            itr = mLines.erase(itr);
            delete line;
        }
        else {
            itr++;
        }
    }
}

void LineRenderer::RednerGrid() {
    unsigned int VAO;
    unsigned int VBO;

    glm::vec3 color = glm::vec3(0.3f, 0.3f, 0.3f);

    int gridSize = 100;

    glUseProgram(mLineShader);

    glm::mat4 MVP = glm::mat4(1.0f);
    glm::mat4 view = CameraComp::GetViewMat();
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f, (float)Window::GetWidth() / (float)Window::GetHeight(), 0.1f, 2000000.0f);
    MVP = projection * view * MVP;


    float vertices[gridSize * 12];
    for(int x = 0; x < gridSize; x++) {
        vertices[x * 6] = x - gridSize/2;
        vertices[x * 6 + 1] = 0.0f;
        vertices[x * 6 + 2] = gridSize/2;

        vertices[x * 6 + 3] = x - gridSize/2;
        vertices[x * 6 + 4] = 0.0f;
        vertices[x * 6 + 5] = -gridSize/2;
    }
    for(int y = 0; y < gridSize; y++) {
        vertices[gridSize * 6 + y * 6] = gridSize/2;
        vertices[gridSize * 6 + y * 6 + 1] = 0.0f;
        vertices[gridSize * 6 + y * 6 + 2] = y - gridSize/2;

        vertices[gridSize * 6 + y * 6 + 3] = -gridSize/2;
        vertices[gridSize * 6 + y * 6 + 4] = 0.0f;
        vertices[gridSize * 6 + y * 6 + 5] = y - gridSize/2;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    glUniformMatrix4fv(glGetUniformLocation(mLineShader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(glGetUniformLocation(mLineShader, "color"), 1, &color[0]);

    glBindVertexArray(VAO);
    glLineWidth(1);
    glDrawArrays(GL_LINES, 0, gridSize * 2);
}

}

