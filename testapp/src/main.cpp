#include <neith.h>

#include <iostream>

#include <stdio.h>
#include <glm/gtx/transform.hpp>
#include <spdlog/spdlog.h>

#define ASSET_DIR "assets/"

int main () {

    //const char *modelPaths[] = {
    //    ASSET_DIR "models/testScene.gltf"
    //};
    
    neith::Window *win = neith::nth_CreateWindow();

    neith::Scene *sc = neith::nth_InitScene(1000);
    neith::nth_AttachSceneToWindow(sc, win);
    //neith::nth_LoadModels(sc, modelPaths, 1);
    int meshCount;
    int meshes = neith::LoadModel(ASSET_DIR "models/testScene.gltf", meshCount);

    //for(int i = 0; i < 50; i++) {
        //for(int j = 0; j < 50; j++) {
            //for(int k = 0; k < 50; k++) {
                //int entity1 = neith::AddEntity("test");
                //glm::mat4 modelMat1 = glm::translate(glm::mat4(1.0f), glm::vec3(i * 10.0f, j * 10.0f, k * 10.0f));
                //neith::AddMeshToEntity(entity1, meshes + 1, modelMat1);
            //}
        //}
    //}
    
    int entity1 = neith::AddEntity("test");
    neith::AddMeshToEntity(entity1, meshes, glm::mat4(1.0f));
    
    neith::Renderer *re = neith::nth_InitRender(sc, win);

    unsigned int shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    //neith::nth_SetShaderByName(sc->mat, "Material", shaderProgram);

    neith::SetShader(meshes, 0, shaderProgram);
    neith::SetShader(meshes, 1, shaderProgram);

    neith::SetValue(meshes, 0, "material.diffuse", 0.4f, 0.4f, 0.4f);
    neith::SetValue(meshes, 0, "material.roughness", 0.0f);
    neith::SetValue(meshes, 0, "material.normal", 0.5f, 0.5f, 0.0f);
    neith::SetValue(meshes, 0, "material.specular", 0.9f);
    neith::SetValue(meshes, 0, "material.metallic", 0.0f);

    neith::SetValue(meshes, 1, "material.diffuse", 1.0f, 0.0f, 0.0f);
    neith::SetValue(meshes, 1, "material.roughness", 0.0f);
    neith::SetValue(meshes, 1, "material.normal", 0.5f, 0.5f, 0.0f);
    neith::SetValue(meshes, 1, "material.specular", 0.9f);
    neith::SetValue(meshes, 1, "material.metallic", 0.0f);
    //neith::nth_SetValueByNameV3v(sc->mat, "Material", "material.normal", 0.5f, 0.5f, 1.0f);
    //neith::nth_SetValueByNameV3v(sc->mat, "Material", "material.diffuse", 0.0f, 1.0f, 1.0f);
    //nth_SetValueByNameF(sc->mat, "Material", "material.roughness", 1.0f);
    //nth_SetValueByNameF(sc->mat, "Material", "material.specular", 0.01f);
    //nth_SetValueByNameF(sc->mat, "Material", "material.metallic", 0.0f);

    //neith::nth_SetShaderByName(sc->mat, "Material.001", shaderProgram);

    //neith::nth_SetValueByNameV3v(sc->mat, "Material.001", "material.diffuse", 1.0f, 1.0f, 1.0f);
    //neith::nth_SetValueByNameF(sc->mat, "Material.001", "material.roughness", 1.0f);
    //neith::nth_SetValueByNameV3v(sc->mat, "Material.001", "material.normal", 0.5f, 0.5f, 0.0f);
    //neith::nth_SetValueByNameF(sc->mat, "Material.001", "material.specular", 0.01f);
    //neith::nth_SetValueByNameF(sc->mat, "Material.001", "material.metallic", 0.0f);

    while(!glfwWindowShouldClose(win->window)) {
        //glm::mat4 moveMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0001f, 0.0f, 0.0f));
        //neith::nth_Transform(1, moveMat);
        neith::nth_UpdateWindow(win);
        neith::nth_UpdateRender(re, sc);
    }
    //glDeleteProgram(shaderProgram);
    
    //DeleteScene(sc);

    glfwTerminate();
    return 0;
}
