#include <glad/glad.h>
#include "framebuffer.h"
#include "staticrenderer.h"
#include "scene/scene.h"
#include "window/window.h"

void InitRender(struct Scene *sc, struct Window *window) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    InitStaticRender(sc, window);
    sc->rc->window = window;

    float screenVertices[] = {
        -1.0f, -1.0f,   0.0f,  0.0f,
         1.0f,  1.0f,   1.0f,  1.0f,
        -1.0f,  1.0f,   0.0f,  1.0f,
        -1.0f, -1.0f,   0.0f,  0.0f,
         1.0f, -1.0f,   1.0f,  0.0f,
         1.0f,  1.0f,   1.0f,  1.0f,
    };

    unsigned int screenVAO, screenVBO;
    glGenBuffers(1, &screenVBO);
    glGenVertexArrays(1, &screenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glBindVertexArray(screenVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texColorBuffer;
    unsigned int fbo = CreatFrameBuffer(1800, 1000, &texColorBuffer);

    unsigned int screenTexture;
    unsigned int intermediateFBO = CreatIntermediateFrameBuffer(1800, 1000, &screenTexture);
    
    unsigned int depthMap;
    unsigned int depthMapFBO = CreatDepthMapFrameBuffer(&depthMap);

    glUseProgram(cubeMapShader);
    glUniform1i(glGetUniformLocation(cubeMapShader, "skybox"), 0);

    return rc;
}

void UpdateRender(struct Scene *sc) {
    if(sc->window.resize) {
        glDeleteFramebuffers(1, &fbo);
        fbo = CreatFrameBuffer(sc->window.width, sc->window.height, &texColorBuffer);
        intermediateFBO = CreatIntermediateFrameBuffer(sc->window.width, sc->window. height, &screenTexture);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    RenderStaticShadows(sc, shadowMapShader);

    //Scene
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    CameraGetViewMat(sc->cd, view);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mat4 projection;
    glm_perspective(GLM_PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f, projection);

    RenderStatic(sc, cbc.width, cbc.height);

    //Cube Map
    mat4 viewCubeMap = GLM_MAT4_IDENTITY_INIT;
    mat3 view3 = GLM_MAT3_IDENTITY_INIT;
    glm_mat4_pick3(view, view3);
    glm_mat4_ins3(view3, viewCubeMap);

    glUseProgram(cubeMapShader);

    glDepthFunc(GL_LEQUAL);

    UniformMat4v(cubeMapShader, "view", viewCubeMap[0]);
    UniformMat4v(cubeMapShader, "projection", projection[0]);
    glBindVertexArray(cubeMapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, cbc.width, cbc.height, 0, 0, cbc.width, cbc.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(cbc.resize){
        glViewport(0, 0, cbc.width, cbc.height);
        cbc.resize = false;
    }

    glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(screenShader);
    glBindVertexArray(screenVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
}


