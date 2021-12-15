#pragma once
#include "staticrenderer.h"
#include "scene/scene.h"

class Renderer {
    StaticRenderer *sr;
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
public:
    Renderer(struct Scene *sc, struct Window *window);
    void UpdateRender(struct Scene *sc);
    ~Renderer();
};
