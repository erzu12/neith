#pragma once

#include <thread>

#include "instancerenderer.h"
#include "linerenderer.h"
#include "shaders.h"

namespace neith {
class Renderer {
public:
    Renderer();
    ~Renderer();

    static void AddShadow(unsigned int shader);
    static void UpdateRender();

private:
    static InstanceRenderer *mInstancedRenderer;
    static Shader mCubeMapShader;
    static Shader mScreenShader;
    static Shader mDepthMapShader;
    static unsigned int mScreenVAO;
    static unsigned int mCubeMapVAO;
    static unsigned int mFBO;
    static unsigned int mDepthMapFBO;
    static unsigned int mDepthMap;
    static unsigned int mIntermediateFBO;
    static unsigned int mTexColorBuffer;
    static unsigned int mScreenTexture;
    static unsigned int mCubeMap;
    std::thread mLODThread;
};
}  // namespace neith
