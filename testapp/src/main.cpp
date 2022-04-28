#include <neith.h>
#include <spdlog/spdlog.h>
#include <stdio.h>

#include <glm/gtx/transform.hpp>
#include <iostream>

#include "camera.h"

#define ASSET_DIR "assets/"

int main()
{
    neith::Window *win = neith::nth_CreateWindow();

    int meshCount;
    int meshes = neith::LoadModel(ASSET_DIR "models/testScene.gltf", meshCount);

    neith::InitScene();
    int entity1 = neith::AddEntity("test");
    neith::AddMeshToEntity(entity1, meshes, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

    Camera camera;


    unsigned int shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    // neith::nth_SetShaderByName(sc->mat, "Material", shaderProgram);
    
    NT_INFO(shaderProgram);

    neith::SetShader(meshes, 0, shaderProgram);
    neith::SetShader(meshes, 1, shaderProgram);

    neith::SetValue(meshes, 0, "material.diffuse", 0.4f, 0.4f, 0.4f);
    neith::SetValue(meshes, 0, "material.roughness", 0.2f);
    neith::SetValue(meshes, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(meshes, 0, "material.specular", 0.2f);
    neith::SetValue(meshes, 0, "material.metallic", 0.2f);

    neith::SetValue(meshes, 1, "material.diffuse", 1.0f, 0.0f, 0.0f);
    neith::SetValue(meshes, 1, "material.roughness", 0.2f);
    neith::SetValue(meshes, 1, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(meshes, 1, "material.specular", 0.9f);
    neith::SetValue(meshes, 1, "material.metallic", 0.0f);
    // neith::nth_SetValueByNameV3v(sc->mat, "Material", "material.normal",
    // 0.5f, 0.5f, 1.0f); neith::nth_SetValueByNameV3v(sc->mat, "Material",
    // "material.diffuse", 0.0f, 1.0f, 1.0f); nth_SetValueByNameF(sc->mat,
    // "Material", "material.roughness", 1.0f); nth_SetValueByNameF(sc->mat,
    // "Material", "material.specular", 0.01f); nth_SetValueByNameF(sc->mat,
    // "Material", "material.metallic", 0.0f);

    // neith::nth_SetShaderByName(sc->mat, "Material.001", shaderProgram);

    // neith::nth_SetValueByNameV3v(sc->mat, "Material.001",
    // "material.diffuse", 1.0f, 1.0f, 1.0f);
    // neith::nth_SetValueByNameF(sc->mat, "Material.001",
    // "material.roughness", 1.0f); neith::nth_SetValueByNameV3v(sc->mat,
    // "Material.001", "material.normal", 0.5f, 0.5f, 0.0f);
    // neith::nth_SetValueByNameF(sc->mat, "Material.001", "material.specular",
    // 0.01f); neith::nth_SetValueByNameF(sc->mat, "Material.001",
    // "material.metallic", 0.0f);

    while (!glfwWindowShouldClose(win->GetGLFWwindow())) {
        camera.UpdateCamera();
        neith::Update();
        //neith::Debug::DrawLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        //neith::Debug::DrawLine(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), neith::Color::magenta, 4.0f);
        //neith::Debug::DrawRay(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 5);
        // glm::mat4 moveMat = glm::translate(glm::mat4(1.0f),
        // glm::vec3(0.0001f, 0.0f, 0.0f)); neith::nth_Transform(1, moveMat);
    }
    // glDeleteProgram(shaderProgram);

    // DeleteScene(sc);

    glfwTerminate();
    return 0;
}
