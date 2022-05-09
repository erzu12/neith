#include "render.h"

#include <glad/glad.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "defaults.h"
#include "framebuffer.h"
#include "scene/components/cameraComp.h"
#include "scene/material.h"
#include "scene/scene.h"
#include "scene/skybox.h"
#include "shaders.h"
#include "textures.h"
#include "window/window.h"

#include "debug.h"

#include "log.h"

namespace neith {

InstanceRenderer *Renderer::mInstancedRenderer;
unsigned int Renderer::mCubeMapShader;
unsigned int Renderer::mScreenShader;
unsigned int Renderer::mShadowMapShader;
unsigned int Renderer::mScreenVAO;
unsigned int Renderer::mCubeMapVAO;
unsigned int Renderer::mFBO;
unsigned int Renderer::mDepthMapFBO;
unsigned int Renderer::mDepthMap;
unsigned int Renderer::mIntermediateFBO;
unsigned int Renderer::mTexColorBuffer;
unsigned int Renderer::mScreenTexture;
unsigned int Renderer::mCubeMap;

Renderer::Renderer()
{
    mInstancedRenderer = new InstanceRenderer();
    LineRenderer::InitLineRenderer();

    mCubeMapShader = LoadAndCompileShaders(NTH_ASSET_DIR "cubeMap.vert", NTH_ASSET_DIR "cubeMap.frag");
    mScreenShader = LoadAndCompileShaders(NTH_ASSET_DIR "screen.vert", NTH_ASSET_DIR "screen.frag");
    mShadowMapShader = LoadAndCompileShaders(NTH_ASSET_DIR "shadowMap.vert", NTH_ASSET_DIR "shadowMap.frag");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    float screenVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,
    };

    unsigned int screenVBO;
    glGenBuffers(1, &screenVBO);
    glGenVertexArrays(1, &mScreenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    glBindVertexArray(mScreenVAO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int cubeMapVBO;
    glGenBuffers(1, &cubeMapVBO);
    glGenVertexArrays(1, &mCubeMapVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), GetSkyboxVertices(), GL_STATIC_DRAW);

    glBindVertexArray(mCubeMapVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Textures
    const char *cubeMapPaths[] = {
        NTH_ASSET_DIR "test/skybox/right.jpg", NTH_ASSET_DIR "test/skybox/left.jpg",
        NTH_ASSET_DIR "test/skybox/top.jpg",   NTH_ASSET_DIR "test/skybox/bottom.jpg",
        NTH_ASSET_DIR "test/skybox/front.jpg", NTH_ASSET_DIR "test/skybox/back.jpg",
    };
    mCubeMap = LoadCubeMap(cubeMapPaths);

    mFBO = CreatFrameBuffer(1800, 1000, &mTexColorBuffer);

    mIntermediateFBO = CreatIntermediateFrameBuffer(1800, 1000, &mScreenTexture);

    mDepthMapFBO = CreatDepthMapFrameBuffer(&mDepthMap);

    Materials::AddDepthMap(mDepthMap);

    glUseProgram(mCubeMapShader);
    glUniform1i(glGetUniformLocation(mCubeMapShader, "skybox"), 0);

    //SetTextureByName(sc->rc->mat, "Material", depthMap, "shadowMap");
    //neith::SetValue(meshes, 0, "material.normal", 0.0f, 0.5f, 0.5f);
}

void Renderer::AddShadow(unsigned int shader) {
    int location = glGetUniformLocation(shader, "shadowMap");
    if(location == -1) {
        NT_INTER_WARN("no binding shadowMap in shader: {}", shader);
        return;
    }
    glUniform1i(location, mDepthMap);
}

void Renderer::UpdateRender()
{
    if (Window::GetResize()) {
        glDeleteFramebuffers(1, &mFBO);
        mFBO = CreatFrameBuffer(Window::GetWidth(), Window::GetHeight(), &mTexColorBuffer);
        mIntermediateFBO = CreatIntermediateFrameBuffer(Window::GetWidth(), Window::GetHeight(), &mScreenTexture);
    }
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    mInstancedRenderer->RenderInstancedShadows(mShadowMapShader);

    // Scene
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glm::mat4 view = CameraComp::GetViewMat();

    int width, height;
    glfwGetWindowSize(Window::GetGLFWwindow(), &width, &height);
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.5f, (float)width / (float)height, 0.1f, 1000.0f);

    mInstancedRenderer->RenderInstanced(Window::GetWidth(), Window::GetHeight());
    LineRenderer::RenderLines();
    LineRenderer::RednerGrid();

    // Cube Map
    glm::mat3 view3 = glm::mat3(view);
    glm::mat4 viewCubeMap = glm::mat4(view3);

    glUseProgram(mCubeMapShader);

    glDepthFunc(GL_LEQUAL);

    UniformMat4v(mCubeMapShader, "view", viewCubeMap);
    UniformMat4v(mCubeMapShader, "projection", projection);
    glBindVertexArray(mCubeMapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mIntermediateFBO);
    glBlitFramebuffer(0, 0, Window::GetWidth(), Window::GetHeight(), 0, 0, Window::GetWidth(), Window::GetHeight(),
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (Window::GetResize()) {
        glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
        Window::SetResize(false);
    }

    glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mScreenShader);
    glBindVertexArray(mScreenVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mScreenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(Window::GetGLFWwindow());
}

Renderer::~Renderer() { delete mInstancedRenderer; }
}  // namespace neith
