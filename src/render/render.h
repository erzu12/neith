#pragma once

#include "instancerenderer.h"
#include "scene/scene.h"

namespace neith {
class Renderer {
public:
    Renderer(Scene *sc, struct Window *window);
    ~Renderer();

    void UpdateRender(Scene *sc);

private:
    InstanceRenderer *sr;
    struct Window *win;
    unsigned int cubeMapShader;
    unsigned int screenShader;
    unsigned int shadowMapShader;
    unsigned int screenVAO;
    unsigned int cubeMapVAO;
    unsigned int FBO;
    unsigned int depthMapFBO;
    unsigned int intermediateFBO;
    unsigned int texColorBuffer;
    unsigned int screenTexture;
    unsigned int cubeMap;
};
}  // namespace neith
