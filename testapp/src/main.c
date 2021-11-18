#include <neith.h>

#include <stdio.h>

#define ASSET_DIR "/media/home/dev/neith/assets/"

int main () {

    const char *modelPaths[] = {
        ASSET_DIR "models/testScene.gltf"
    };
    struct Window *win = nth_CreateWindow();

    struct Scene *sc = InitScene(1000);
    //get camera data in callbackContext
    nth_LoadModels(sc, modelPaths, 1);
    
    nth_InitRender(sc, win);

    unsigned int shaderProgram = nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    nth_SetShaderByName(sc->rc->mat, "Material", shaderProgram);

    nth_SetValueByNameV3v(sc->rc->mat, "Material", "material.diffuse", 1.0f, 1.0f, 1.0f);
    nth_SetValueByNameF(sc->rc->mat, "Material", "material.roughness", 1.0f);
    nth_SetValueByNameV3v(sc->rc->mat, "Material", "material.normal", 0.5f, 0.5f, 0.0f);
    nth_SetValueByNameF(sc->rc->mat, "Material", "material.specular", 0.01f);
    nth_SetValueByNameF(sc->rc->mat, "Material", "material.metallic", 0.0f);

    while(!glfwWindowShouldClose(win->window)) {
        nth_UpdateWindow(win);
        nth_UpdateRender(sc);
    }
    glDeleteProgram(shaderProgram);
    
    DeleteScene(sc);

    glfwTerminate();
    return 0;
}
