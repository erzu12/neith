
#include <spdlog/spdlog.h>
#include <stdio.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <random>

#include "camera.h"

#define ASSET_DIR "assets/"

int main()
{
    // neith::Window *win = neith::nth_CreateWindow();
    neith::Init();

    // neith::Model *planeMesh = neith::LoadModel(ASSET_DIR "models/plane.gltf");
    // neith::Model *skyScraper = neith::LoadModel(ASSET_DIR "models/RuinedCitySkyRise04.gltf");

    unsigned int mesh = neith::AddMesh(1);
    float dists[1] = { 1000000.0f };
    neith::SetLODs(mesh, 1, dists);

    int size = 1000;

    int vertCount = size * size;
    int indCount = (size - 1) * (size - 1) * 6;
    float *vertices = new float[vertCount * 12];
    int *indices = new int[indCount];

    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++) {
            vertices[(x * size + z) * 12 + 0] = (float)x * 1.0 - size / 2;
            vertices[(x * size + z) * 12 + 1] =
                20 * neith::OpenSimplex2D(((double)x - size / 2) * 0.001f, ((double)z - size / 2) * 0.001f, 4, 0.6);
            vertices[(x * size + z) * 12 + 2] = (float)z * 1.0 - size / 2;

            vertices[(x * size + z) * 12 + 10] = (float)x * x / size;
            vertices[(x * size + z) * 12 + 11] = (float)z * z / size;
        }
    }

    for (int x = 0; x < size - 1; x++) {
        for (int z = 0; z < size - 1; z++) {
            indices[(x * (size - 1) + z) * 6 + 0] = x * size + z;
            indices[(x * (size - 1) + z) * 6 + 1] = x * size + z + 1;
            indices[(x * (size - 1) + z) * 6 + 2] = x * size + z + size + 1;

            indices[(x * (size - 1) + z) * 6 + 3] = x * size + z;
            indices[(x * (size - 1) + z) * 6 + 4] = x * size + z + size + 1;
            indices[(x * (size - 1) + z) * 6 + 5] = x * size + z + size;
        }
    }

    neith::CalcNormals(vertices, vertCount, indices, indCount);
    neith::CalcTangents(vertices, vertCount, indices, indCount);

    unsigned int groundMat = neith::AddMaterial();
    neith::AddPrimitive(vertices, vertCount, indices, indCount, mesh, 0, groundMat);

    neith::Model *cubeMesh = neith::LoadModel(ASSET_DIR "models/cube.gltf");
    // neith::Model *treeMesh = neith::LoadModel(ASSET_DIR "models/Tree1.gltf");
    neith::Model *grassMesh = neith::LoadModel(ASSET_DIR "models/Grass.gltf");

    neith::InitRenderer();

    auto start = std::chrono::steady_clock::now();
    std::chrono::duration<double> total = start - start;
    std::mt19937 gen;  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < 1000000; i++) {
        float x = (float)distr(gen);
        float y = (float)distr(gen);
        float rotate = (float)distr(gen);

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f),
                           glm::vec3(x, 20 * neith::OpenSimplex2D((double)x * 0.001f, (double)y * 0.001f, 4, 0.6), y));
        transform *= glm::rotate(rotate, glm::vec3(0.0f, 1.0f, 0.0f));

        int treeEntity = neith::AddEntity("tree" + std::to_string(i), transform);
        start = std::chrono::steady_clock::now();
        neith::AddModelToEntity(treeEntity, grassMesh);

        auto end = std::chrono::steady_clock::now();
        total += end - start;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    NT_INFO("Ran in {}s", total.count());

    int meshEntity = neith::AddEntity("mesh");
    neith::AddMeshToEntity(meshEntity, mesh);

    int cubeEntity = neith::AddEntity("cube", glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 2.0f, 5.0f)));
    neith::AddModelToEntity(cubeEntity, cubeMesh);

    // int cube = neith::AddEntity("cube", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f)));
    // neith::AddMeshToEntity(cube, cubeMesh);
    // int cube1 = neith::AddEntity("cube1", glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 2.0f, 0.0f)));
    // neith::AddMeshToEntity(cube1, cubeMesh);

    // for (int i = 0; i < meshCount; i++) {
    // int entity = neith::AddEntity("skyScraper", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    // neith::AddModelToEntity(entity, skyScraper);
    // }

    // int plane = neith::AddEntity("plane");
    // neith::AddModelToEntity(plane, planeMesh);

    Camera camera;

    unsigned int shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    // neith::nth_SetShaderByName(sc->mat, "Material", shaderProgram);

    // unsigned int albedo = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Albedo.jpg", GL_RGB, GL_RGB);
    // unsigned int normal = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Normal.jpg", GL_RGB, GL_RGB);
    // unsigned int roughness = neith::LoadTexture(ASSET_DIR "textures/Concrete_Dirty/Roughness.jpg", GL_RGB, GL_RGB);
    unsigned int grassAlpha = neith::LoadTexture(ASSET_DIR "textures/grassAlpha.jpg", GL_RGB, GL_RGB);

    // unsigned int ivy = neith::LoadTexture(ASSET_DIR "textures/ivy-png.png", GL_RGBA, GL_RGBA);
    // unsigned int ivyAlpha = neith::LoadTexture(ASSET_DIR "textures/ivy-png-alpha.png", GL_RGBA, GL_RGBA);
    //   neith::BoxCollider *boxCollider = new neith::BoxCollider(2.f, 2.f, 2.f);
    //   unsigned int boxRigidBody = neith::AddRigidBody(cube, boxCollider, 1.f);
    //   neith::AddRigidBody(cube1, boxCollider, 1.f);
    //   neith::AddRigidBody(plane, new neith::BoxCollider(20.f, 0.0001f, 20.f), 0.f);

    // neith::SetShader(skyScraper, 0, shaderProgram);
    // neith::SetShader(meshes, 1, shaderProgram);
    neith::SetShader(groundMat, shaderProgram);

    neith::SetValue(mesh, 0, "material.diffuse", 0.1f, 0.6f, 0.1f);
    neith::SetValue(mesh, 0, "material.roughness", 0.8f);
    neith::SetValue(mesh, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(mesh, 0, "material.specular", 0.2f);
    neith::SetValue(mesh, 0, "material.metallic", 0.0f);

    neith::SetShader(cubeMesh, 0, shaderProgram);

    neith::SetValue(cubeMesh, 0, "material.diffuse", 0.5f, 0.5f, 0.5f);
    neith::SetValue(cubeMesh, 0, "material.roughness", 0.8f);
    neith::SetValue(cubeMesh, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(cubeMesh, 0, "material.specular", 0.2f);
    neith::SetValue(cubeMesh, 0, "material.metallic", 0.0f);

    // neith::SetShader(treeMesh, 0, shaderProgram);

    // neith::SetValue(treeMesh, 0, "material.diffuse", 0.1f, 0.04f, 0.0f);
    // neith::SetValue(treeMesh, 0, "material.roughness", 0.8f);
    // neith::SetValue(treeMesh, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(treeMesh, 0, "material.specular", 0.2f);
    // neith::SetValue(treeMesh, 0, "material.metallic", 0.0f);

    // neith::SetShader(treeMesh, 1, shaderProgram);

    // neith::SetValue(treeMesh, 1, "material.diffuse", 0.0f, 0.3f, 0.1f);
    // neith::SetValue(treeMesh, 1, "material.roughness", 0.8f);
    // neith::SetValue(treeMesh, 1, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(treeMesh, 1, "material.specular", 0.2f);
    // neith::SetValue(treeMesh, 1, "material.metallic", 0.0f);

    neith::SetShader(grassMesh, 0, shaderProgram);
    neith::MakeBackfaced(grassMesh, 0);

    neith::SetValue(grassMesh, 0, "material.diffuse", 0.1f, 0.6f, 0.1f);
    neith::SetValue(grassMesh, 0, "material.roughness", 0.8f);
    neith::SetValue(grassMesh, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    neith::SetValue(grassMesh, 0, "material.specular", 0.2f);
    neith::SetValue(grassMesh, 0, "material.metallic", 0.0f);
    // neith::SetTransparancyTexture(grassMesh, 0, grassAlpha);

    // neith::SetTexture(skyScraper, 0, albedo, "material.diffuse");
    // neith::SetTexture(skyScraper, 0, roughness, "material.roughness");
    // neith::SetTexture(skyScraper, 0, normal, "material.normal");
    // neith::SetValue(skyScraper, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(skyScraper, 0, "material.specular", 0.2f);
    // neith::SetValue(skyScraper, 0, "material.metallic", 0.2f);

    // neith::SetShader(skyScraper, 3, shaderProgram);

    // neith::SetTexture(skyScraper, 3, ivy, "material.diffuse");
    // neith::SetValue(skyScraper, 3, "material.roughness", 0.5f);
    // neith::SetValue(skyScraper, 3, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(skyScraper, 0, "material.normal", 0.5f, 0.5f, 1.0f);
    // neith::SetValue(skyScraper, 3, "material.specular", 0.2f);
    // neith::SetValue(skyScraper, 3, "material.metallic", 0.2f);
    // neith::SetTransparancyTexture(skyScraper, 3, ivyAlpha);
    //   neith::SetValue(meshes, 1, "material.diffuse", 1.0f, 0.0f, 0.0f);
    //   neith::SetValue(meshes, 1, "material.roughness", 0.2f);
    //   neith::SetValue(meshes, 1, "material.normal", 0.5f, 0.5f, 1.0f);
    //   neith::SetValue(meshes, 1, "material.specular", 0.9f);
    //   neith::SetValue(meshes, 1, "material.metallic", 0.0f);

    // delete skyScraper;
    // delete planeMesh;
    // neith::ContactPoint *contactPoints = new neith::ContactPoint[4];

    while (!neith::ShouldClose()) {
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
