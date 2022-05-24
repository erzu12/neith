
#include <spdlog/spdlog.h>
#include <stdio.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "camera.h"

#define ASSET_DIR "assets/"

int main()
{
    neith::Window *win = neith::nth_CreateWindow();

    int meshCount;
    // int cubeMesh = neith::LoadModel(ASSET_DIR "models/cube.gltf", meshCount);
    neith::Model *planeMesh = neith::LoadModel(ASSET_DIR "models/plane.gltf", meshCount);
    neith::Model *skyScraper = neith::LoadModel(ASSET_DIR "models/RuinedCitySkyRise04.gltf", meshCount);

    neith::InitScene();
    // int cube = neith::AddEntity("cube", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f)));
    // neith::AddMeshToEntity(cube, cubeMesh);
    // int cube1 = neith::AddEntity("cube1", glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 2.0f, 0.0f)));
    // neith::AddMeshToEntity(cube1, cubeMesh);

    // for (int i = 0; i < meshCount; i++) {
    int entity = neith::AddEntity("skyScraper", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    neith::AddMeshToEntity(entity, skyScraper[0]);
    //}

    int plane = neith::AddEntity("plane");
    neith::AddMeshToEntity(plane, planeMesh[0]);

    Camera camera;

    unsigned int shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    // neith::nth_SetShaderByName(sc->mat, "Material", shaderProgram);

    unsigned int albedo = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Albedo.jpg", GL_RGB, GL_RGB);
    unsigned int normal = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Normal.jpg", GL_RGB, GL_RGB);
    unsigned int roughness = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Roughness.jpg", GL_RGB, GL_RGB);
    // neith::BoxCollider *boxCollider = new neith::BoxCollider(2.f, 2.f, 2.f);
    // unsigned int boxRigidBody = neith::AddRigidBody(cube, boxCollider, 1.f);
    // neith::AddRigidBody(cube1, boxCollider, 1.f);
    // neith::AddRigidBody(plane, new neith::BoxCollider(20.f, 0.0001f, 20.f), 0.f);

    neith::SetShader(skyScraper[0], 0, shaderProgram);
    // neith::SetShader(meshes, 1, shaderProgram);

    neith::SetTexture(skyScraper[0], 0, albedo, "material.diffuse");
    neith::SetTexture(skyScraper[0], 0, roughness, "material.roughness");
    neith::SetTexture(skyScraper[0], 0, normal, "material.normal");
    // neith::SetValue(skyScraper, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(skyScraper[0], 0, "material.specular", 0.2f);
    neith::SetValue(skyScraper[0], 0, "material.metallic", 0.2f);

    // neith::SetValue(meshes, 1, "material.diffuse", 1.0f, 0.0f, 0.0f);
    // neith::SetValue(meshes, 1, "material.roughness", 0.2f);
    // neith::SetValue(meshes, 1, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(meshes, 1, "material.specular", 0.9f);
    // neith::SetValue(meshes, 1, "material.metallic", 0.0f);

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

    delete skyScraper;
    delete planeMesh;
    // neith::ContactPoint *contactPoints = new neith::ContactPoint[4];

    while (!glfwWindowShouldClose(win->GetGLFWwindow())) {
        camera.UpdateCamera();
        neith::Update();
        // int contactPointCount = neith::GetContacPoints(boxRigidBody, contactPoints, 4);
        // for (int i = 0; i < contactPointCount; i++) {
        // NT_INFO(glm::to_string(contactPoints[i].worldNormal));
        //}
        // NT_INFO(contactPointCount);
        // neith::Debug::DrawLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        // neith::Debug::DrawLine(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f),
        // neith::Color::magenta, 4.0f); neith::Debug::DrawRay(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
        // 5); glm::mat4 moveMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0001f, 0.0f, 0.0f));
        // neith::nth_Transform(1, moveMat);
    }
    // glDeleteProgram(shaderProgram);

    // DeleteScene(sc);

    glfwTerminate();
    return 0;
}
