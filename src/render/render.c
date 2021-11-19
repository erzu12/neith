#include "render.h"
#include <glad/glad.h>
#include "framebuffer.h"
#include "staticrenderer.h"
#include "scene/skybox.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "window/window.h"
#include "shaders.h"
#include "textures.h"

#define ASSET_DIR "/media/ssd2/dev/neith/assets/"

void InitRender(struct Scene *sc, struct Window *window) {
    InitStaticRender(sc, window);

    sc->rc->cubeMapShader = LoadAndCompileShaders(ASSET_DIR "cubeMap.vert", ASSET_DIR "cubeMap.frag");
    sc->rc->screenShader = LoadAndCompileShaders(ASSET_DIR "screen.vert", ASSET_DIR "screen.frag");
    sc->rc->shadowMapShader = LoadAndCompileShaders(ASSET_DIR "shadowMap.vert", 
                                                         ASSET_DIR "shadowMap.frag"); 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    sc->rc->win = window;

    float screenVertices[] = {
        -1.0f, -1.0f,   0.0f,  0.0f,
         1.0f,  1.0f,   1.0f,  1.0f,
        -1.0f,  1.0f,   0.0f,  1.0f,
        -1.0f, -1.0f,   0.0f,  0.0f,
         1.0f, -1.0f,   1.0f,  0.0f,
         1.0f,  1.0f,   1.0f,  1.0f,
    };

    unsigned int screenVBO;
    glGenBuffers(1, &screenVBO);
    glGenVertexArrays(1, &sc->rc->screenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glBindVertexArray(sc->rc->screenVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int cubeMapVBO;
    glGenBuffers(1, &cubeMapVBO);
    glGenVertexArrays(1, &sc->rc->cubeMapVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), GetSkyboxVertices(), GL_STATIC_DRAW);

    glBindVertexArray(sc->rc->cubeMapVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Textures
    const char *cubeMapPaths[] = {
        ASSET_DIR "test/skybox/right.jpg",
        ASSET_DIR "test/skybox/left.jpg",
        ASSET_DIR "test/skybox/top.jpg",
        ASSET_DIR "test/skybox/bottom.jpg",
        ASSET_DIR "test/skybox/front.jpg",
        ASSET_DIR "test/skybox/back.jpg",
    };
    sc->rc->cubeMap = LoadCubeMap(cubeMapPaths);

    sc->rc->FBO = CreatFrameBuffer(1800, 1000, &sc->rc->texColorBuffer);

    sc->rc->intermediateFBO = CreatIntermediateFrameBuffer(1800, 1000, &sc->rc->screenTexture);
    
    unsigned int depthMap;
    sc->rc->depthMapFBO = CreatDepthMapFrameBuffer(&depthMap);

    glUseProgram(sc->rc->cubeMapShader);
    glUniform1i(glGetUniformLocation(sc->rc->cubeMapShader, "skybox"), 0);
}

void UpdateRender(struct Scene *sc) {
    if(sc->rc->win->resize) {
        glDeleteFramebuffers(1, &sc->rc->FBO);
        sc->rc->FBO= CreatFrameBuffer(sc->rc->win->width, sc->rc->win->height, &sc->rc->texColorBuffer);
        sc->rc->intermediateFBO = CreatIntermediateFrameBuffer(sc->rc->win->width, sc->rc->win->height, &sc->rc->screenTexture);
    }
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, sc->rc->depthMapFBO);
    RenderStaticShadows(sc, sc->rc->shadowMapShader);

    //Scene
    glBindFramebuffer(GL_FRAMEBUFFER, sc->rc->FBO);
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    CameraGetViewMat(sc->cd, view);

    int width, height;
    glfwGetWindowSize(sc->rc->win->window, &width, &height);
    mat4 projection;
    glm_perspective(GLM_PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f, projection);

    RenderStatic(sc, sc->rc->win->width, sc->rc->win->height);

    //Cube Map
    mat4 viewCubeMap = GLM_MAT4_IDENTITY_INIT;
    mat3 view3 = GLM_MAT3_IDENTITY_INIT;
    glm_mat4_pick3(view, view3);
    glm_mat4_ins3(view3, viewCubeMap);

    glUseProgram(sc->rc->cubeMapShader);

    glDepthFunc(GL_LEQUAL);

    UniformMat4v(sc->rc->cubeMapShader, "view", viewCubeMap[0]);
    UniformMat4v(sc->rc->cubeMapShader, "projection", projection[0]);
    glBindVertexArray(sc->rc->cubeMapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sc->rc->cubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sc->rc->FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sc->rc->intermediateFBO);
    glBlitFramebuffer(0, 0, sc->rc->win->width, sc->rc->win->height, 0, 0, sc->rc->win->width, sc->rc->win->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(sc->rc->win->resize){
        glViewport(0, 0, sc->rc->win->width, sc->rc->win->height);
        sc->rc->win->resize = false;
    }

    glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sc->rc->screenShader);
    glBindVertexArray(sc->rc->screenVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sc->rc->screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(sc->rc->win->window);
}


