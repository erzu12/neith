#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "vecmath.h"

#include "render.h"
#include <glad/glad.h>
#include "framebuffer.h"
#include "scene/skybox.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/material.h"
#include "window/window.h"
#include "shaders.h"
#include "textures.h"
#include "defaults.h"
namespace neith {
    Renderer::Renderer(struct Scene *sc, struct Window *window) {
        sr = new InstanceRenderer(sc, window);

        cubeMapShader = LoadAndCompileShaders(NTH_ASSET_DIR "cubeMap.vert", NTH_ASSET_DIR "cubeMap.frag");
        screenShader = LoadAndCompileShaders(NTH_ASSET_DIR "screen.vert", NTH_ASSET_DIR "screen.frag");
        shadowMapShader = LoadAndCompileShaders(NTH_ASSET_DIR "shadowMap.vert", 
                                                             NTH_ASSET_DIR "shadowMap.frag"); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);

        win = window;

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
        glGenVertexArrays(1, &screenVAO);

        glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

        glBindVertexArray(screenVAO);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        unsigned int cubeMapVBO;
        glGenBuffers(1, &cubeMapVBO);
        glGenVertexArrays(1, &cubeMapVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
        glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), GetSkyboxVertices(), GL_STATIC_DRAW);

        glBindVertexArray(cubeMapVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //Textures
        const char *cubeMapPaths[] = {
            NTH_ASSET_DIR "test/skybox/right.jpg",
            NTH_ASSET_DIR "test/skybox/left.jpg",
            NTH_ASSET_DIR "test/skybox/top.jpg",
            NTH_ASSET_DIR "test/skybox/bottom.jpg",
            NTH_ASSET_DIR "test/skybox/front.jpg",
            NTH_ASSET_DIR "test/skybox/back.jpg",
        };
        cubeMap = LoadCubeMap(cubeMapPaths);

        FBO = CreatFrameBuffer(1800, 1000, &texColorBuffer);

        intermediateFBO = CreatIntermediateFrameBuffer(1800, 1000, &screenTexture);
        
        unsigned int depthMap;
        depthMapFBO = CreatDepthMapFrameBuffer(&depthMap);

        glUseProgram(cubeMapShader);
        glUniform1i(glGetUniformLocation(cubeMapShader, "skybox"), 0);

        //SetTextureByName(sc->rc->mat, "Material", depthMap, "shadowMap");
    }

    void Renderer::UpdateRender(struct Scene *sc) {
        if(win->resize) {
            glDeleteFramebuffers(1, &FBO);
            FBO = CreatFrameBuffer(win->width, win->height, &texColorBuffer);
            intermediateFBO = CreatIntermediateFrameBuffer(win->width, win->height, &screenTexture);
        }
        glEnable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        sr->RenderInstancedShadows(sc, shadowMapShader);

        //Scene
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glm::mat4 view = glm::mat4(1.0f);
        sc->cd->CameraGetViewMat(view);

        int width, height;
        glfwGetWindowSize(win->window, &width, &height);
        glm::mat4 projection = glm::perspective(PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f);

        sr->RenderInstanced(sc, win->width, win->height);

        //Cube Map
        glm::mat3 view3 = glm::mat3(view);
        glm::mat4 viewCubeMap = glm::mat4(view3);

        glUseProgram(cubeMapShader);

        glDepthFunc(GL_LEQUAL);

        UniformMat4v(cubeMapShader, "view", viewCubeMap);
        UniformMat4v(cubeMapShader, "projection", projection);
        glBindVertexArray(cubeMapVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
        glBlitFramebuffer(0, 0, win->width, win->height, 0, 0, win->width, win->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if(win->resize){
            glViewport(0, 0, win->width, win->height);
            win->resize = false;
        }

        glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(screenShader);
        glBindVertexArray(screenVAO);
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(win->window);
    }

    Renderer::~Renderer() {
        delete sr;
    }
}
