#include "render.h"

#include <glad/glad.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <thread>

#include "debug.h"
#include "defaults.h"
#include "framebuffer.h"
#include "gui/gui.h"
#include "log.h"
#include "scene/components/cameraComp.h"
#include "scene/material.h"
#include "scene/skybox.h"
#include "shaders.h"
#include "textures.h"
#include "window/window.h"

namespace neith {

InstanceRenderer *Renderer::mInstancedRenderer;
Shader Renderer::mCubeMapShader;
Shader Renderer::mScreenShader;
Shader Renderer::mDepthMapShader;
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

    mCubeMapShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "cubeMap.vert", NTH_ASSET_DIR "cubeMap.frag");
    mScreenShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "screen.vert", NTH_ASSET_DIR "screen.frag");
    mDepthMapShader = Shader::LoadAndCompileShaders(NTH_ASSET_DIR "shadowMap.vert", NTH_ASSET_DIR "shadowMap.frag");
    Material::setDepthShader(mDepthMapShader);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    int maxCombinedTextureUnits;
    int maxTextureUnits;
    int maxTextureSize;
    int maxWorkGroupCount[3];
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxWorkGroupCount[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxWorkGroupCount[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxWorkGroupCount[2]);

    NT_INTER_INFO("max combined texture image units: {}", maxCombinedTextureUnits);
    NT_INTER_INFO("max texture image units: {}", maxTextureUnits);
    NT_INTER_INFO("max texture size: {}", maxTextureSize);
    NT_INTER_INFO("max work group count: {} {} {}", maxWorkGroupCount[0], maxWorkGroupCount[1], maxWorkGroupCount[2]);

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

    glUseProgram(mCubeMapShader.mShaderProgram);
    glUniform1i(glGetUniformLocation(mCubeMapShader.mShaderProgram, "skybox"), 0);

    Gui::Init();

    //mLODThread = std::thread(system::ContinuousLODUpdate);

    // SetTextureByName(sc->rc->mat, "Material", depthMap, "shadowMap");
    // neith::SetValue(meshes, 0, "material.normal", 0.0f, 0.5f, 0.5f);
}

void Renderer::AddShadow(unsigned int shader)
{
    int location = glGetUniformLocation(shader, "shadowMap");
    if (location == -1) {
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

    int width, height;
    glfwGetWindowSize(Window::GetGLFWwindow(), &width, &height);

    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    //mInstancedRenderer->RenderInstancedShadows(mDepthMapShader, mDepthMap);

    // Scene
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glm::mat4 view = CameraComp::GetViewMat();

    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.5f, (float)width / (float)height, 0.1f, 1000.0f);

    mInstancedRenderer->renderSystem(Window::GetWidth(), Window::GetHeight(), mDepthMap);
    LineRenderer::RenderLines();

    // Cube Map
    glm::mat3 view3 = glm::mat3(view);
    glm::mat4 viewCubeMap = glm::mat4(view3);

    // glUseProgram(mCubeMapShader);

    glDepthFunc(GL_LEQUAL);

    // UniformMat4v(mCubeMapShader, "view", viewCubeMap);
    // UniformMat4v(mCubeMapShader, "projection", projection);
    // glBindVertexArray(mCubeMapVAO);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
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

    glUseProgram(mScreenShader.mShaderProgram);
    glBindVertexArray(mScreenVAO);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mScreenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // system::UpdateLODs();

    Gui::Update();

    glfwSwapBuffers(Window::GetGLFWwindow());
}

Renderer::~Renderer()
{
    mLODThread.join();
    delete mInstancedRenderer;
}




}  // namespace neith
