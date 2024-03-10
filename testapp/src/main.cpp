#include <neith.h>
#include <spdlog/spdlog.h>
#include <stdio.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <random>

#include "perf.h"
#include "camera.h"

#define ASSET_DIR "assets/"

int main()
{

    // neith::Window *win = neith::nth_CreateWindow();
    neith::Init();

    //neith::Shader shader = neith::Shader::LoadAndCompileComputeShader(ASSET_DIR "lod.comp");

    //float data[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    //for (int i = 0; i < sizeof(data) / sizeof(unsigned int); i++) {
        //NT_INFO("{}", data[i]);
    //}

    //unsigned int dataBuffer;
    //glGenBuffers(1, &dataBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    //glUseProgram(shader.mShaderProgram);
    //glUniform1f(glGetUniformLocation(shader.mShaderProgram, "multiplier"), 2.0f);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);

    //glDispatchCompute(sizeof(data), 1, 1);

    //glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
    
    //for (int i = 0; i < sizeof(data) / sizeof(unsigned int); i++) {
        //NT_INFO("{}", data[i]);
    //}

    //return 0;

    // neith::Model *planeMesh = neith::LoadModel(ASSET_DIR "models/plane.gltf");
    // neith::Model *skyScraper = neith::LoadModel(ASSET_DIR "models/RuinedCitySkyRise04.gltf");

    //unsigned int ground = neith::AddMesh(1);
    neith::Mesh *ground = new neith::Mesh();
    //float dists[1] = { 1000000.0f };
    //neith::SetLODs(ground, 1, dists);
    //ground->setLODs({1000000.0f});

    int size = 1000;

    int vertCount = size * size;
    int indCount = (size - 1) * (size - 1) * 6;
    //float *vertices = new float[vertCount * 12];
    //int *indices = new int[indCount];
    std::vector<float> vertices(vertCount * 12);
    std::vector<int> indices(indCount);

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

    neith::CalcNormals(vertices.data(), vertCount, indices.data(), indCount);
    neith::CalcTangents(vertices.data(), vertCount, indices.data(), indCount);

    //unsigned int groundMat = neith::AddMaterial();
    neith::Material groundMat = neith::Material();
    ground->getLOD(0)->AddPrimitive(vertices, indices, &groundMat);

    neith::Model *cubeMesh = neith::LoadModel(ASSET_DIR "models/cube.gltf");
    neith::Model *treeMesh = neith::LoadModel(ASSET_DIR "models/Tree1.gltf");
    //neith::Model *grassMesh = neith::LoadModel(ASSET_DIR "models/Grass.gltf");

    auto start = std::chrono::steady_clock::now();
    std::chrono::duration<double> total = start - start;
    std::mt19937 gen;  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distr(-500, 500);
    NT_INTER_INFO("rand gen done");
    std::vector<glm::mat4> transforms;
    for (int i = 0; i < 10000; i++) {
        float x = (float)distr(gen);
        float y = (float)distr(gen);
        float rotate = (float)distr(gen);

        transforms.push_back(
            glm::translate(glm::mat4(1.0f), 
                glm::vec3(x, 20 * neith::OpenSimplex2D((double)x * 0.001f, (double)y * 0.001f, 4, 0.6), y)));

        start = std::chrono::steady_clock::now();

        auto end = std::chrono::steady_clock::now();
        total += end - start;
    }
    treeMesh->setInstances(transforms);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    NT_INFO("Ran in {}s", total.count());

    //int groundMeshEntity = neith::AddEntity("ground");
    //neith::AddMeshToEntity(groundMeshEntity, ground);

    ground->setInstances({glm::mat4(1.0f)});

    // int cubeEntity = neith::AddEntity("cube", glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 2.0f, 5.0f)));
    // neith::AddModelToEntity(cubeEntity, cubeMesh);

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

    neith::Shader shaderProgram = neith::nth_LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
    neith::Shader defaultShader = neith::nth_LoadAndCompileShaders(ASSET_DIR "default.vert", ASSET_DIR "default.frag");
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
    //neith::SetShader(groundMat, shaderProgram);
    shaderProgram.AddShadows();
    groundMat.setShader(shaderProgram);

    groundMat.setValue("material.diffuse", 0.1f, 0.4f, 0.1f);
    groundMat.setValue("material.roughness", 0.8f);
    groundMat.setValue("material.normal", 0.5f, 0.5f, 1.0f);
    groundMat.setValue("material.specular", 0.2f);
    groundMat.setValue("material.metallic", 0.0f);


    neith::Material *stemMat = treeMesh->getMaterials()[0];
    neith::Material *leafMat = treeMesh->getMaterials()[1];

    stemMat->setShader(shaderProgram);

    stemMat->setValue("material.diffuse", 0.1f, 0.04f, 0.0f);
    stemMat->setValue("material.roughness", 0.8f);
    stemMat->setValue("material.normal", 0.5f, 0.5f, 1.0f);
    stemMat->setValue("material.specular", 0.2f);
    stemMat->setValue("material.metallic", 0.0f);
    stemMat->setValue("material.alpha", 1.0f);

    leafMat->setShader(shaderProgram);

    leafMat->setValue("material.diffuse", 0.0f, 0.3f, 0.1f);
    leafMat->setValue("material.roughness", 0.8f);
    leafMat->setValue("material.normal", 0.5f, 0.5f, 1.0f);
    leafMat->setValue("material.specular", 0.2f);
    leafMat->setValue("material.metallic", 0.0f);
    leafMat->setValue("material.alpha", 1.0f);


    // neith::ContactPoint *contactPoints = new neith::ContactPoint[4];

    while (!neith::ShouldClose()) {
        camera.UpdateCamera();
        double time = glfwGetTime();
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
