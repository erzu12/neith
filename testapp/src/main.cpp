#include <neith.h>

#include <stdio.h>

#define ASSET_DIR "assets/"

int main () {

    const char *modelPaths[] = {
        ASSET_DIR "models/testScene.gltf"
    };
    struct Window *win = neith::nth_CreateWindow();

    struct Scene *sc = neith::nth_InitScene(1000);
    neith::nth_AttachSceneToWindow(sc, win);
    neith::nth_LoadModels(sc, modelPaths, 1);
    
    neith::nth_InitRender(sc, win);

    unsigned int shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    neith::nth_SetShaderByName(sc->rc->mat, "Material", shaderProgram);

    neith::nth_SetValueByNameV3v(sc->rc->mat, "Material", "material.normal", 0.5f, 0.5f, 1.0f);
    neith::nth_SetValueByNameV3v(sc->rc->mat, "Material", "material.diffuse", 0.0f, 1.0f, 1.0f);
    //nth_SetValueByNameF(sc->rc->mat, "Material", "material.roughness", 1.0f);
    //nth_SetValueByNameF(sc->rc->mat, "Material", "material.specular", 0.01f);
    //nth_SetValueByNameF(sc->rc->mat, "Material", "material.metallic", 0.0f);

    neith::nth_SetShaderByName(sc->rc->mat, "Material.001", shaderProgram);

    neith::nth_SetValueByNameV3v(sc->rc->mat, "Material.001", "material.diffuse", 1.0f, 1.0f, 1.0f);
    neith::nth_SetValueByNameF(sc->rc->mat, "Material.001", "material.roughness", 1.0f);
    neith::nth_SetValueByNameV3v(sc->rc->mat, "Material.001", "material.normal", 0.5f, 0.5f, 0.0f);
    neith::nth_SetValueByNameF(sc->rc->mat, "Material.001", "material.specular", 0.01f);
    neith::nth_SetValueByNameF(sc->rc->mat, "Material.001", "material.metallic", 0.0f);

    while(!glfwWindowShouldClose(win->window)) {
        neith::nth_UpdateWindow(win);
        neith::nth_UpdateRender(sc);
    }
    //glDeleteProgram(shaderProgram);
    
    //DeleteScene(sc);

    glfwTerminate();
    return 0;
}
