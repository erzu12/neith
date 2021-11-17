#include <neith.h>

#include <stdio.h>

#define ASSET_DIR "assets/"

int main () {

    const char *modelPaths[] = {
        ASSET_DIR "test/shadowTest2.gltf"
    };
    GLTFwindow *window = nth_CreateWindow();

    struct Scene *sc = InitScene(1000);
    //get camera data in callbackContext
    nth_LoadModels(sc, modelPaths, 1);
    
    nth_InitRender(sc, window);

    unsigned int shaderProgram = LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    SetShaderByName(sc->rc->mat, "Material", shaderProgram);

    SetValueByNameV3v(sc->rc->mat, "Material", "material.diffuse", 1.0f, 1.0f, 1.0f);
    SetValueByNameF(sc->rc->mat, "Material", "material.roughness", 1.0f);
    SetValueByNameV3v(sc->rc->mat, "Material", "material.normal", 0.5f, 0.5f, 0.0f);
    SetValueByNameF(sc->rc->mat, "Material", "material.specular", 0.01f);
    SetValueByNameF(sc->rc->mat, "Material", "material.metallic", 0.0f);

    while(!glfwWindowShouldClose(window)) {
        nth_UpdateWindow(window);
        nth_UpdateRender(sc);
    }
    glDeleteProgram(shaderProgram);
    
    DeleteScene(sc);

    glfwTerminate();
    return 0;
}
