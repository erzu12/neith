#include "instancerenderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "dataStructures.h"
#include "scene/components/cameraComp.h"
#include "scene/components/meshComp.h"
#include "scene/material.h"
#include "scene/mesh.h"
#include "scene/scene.h"
#include "shaders.h"
#include "textures.h"
#include "window/window.h"
#define GLM_FORCE_RADIANS

#include <glad/glad.h>

#include "log.h"

namespace neith {

InstanceRenderer::InstanceRenderer()
{
    // StaticPrimitives *sp = sc->sp;
    // struct RenderContext *rc = (struct RenderContext*)malloc(sizeof(struct
    // RenderContext));
    VAOs = new unsigned int[MeshComp::GetPrimitivesCount()];
    VBOs = Alloc2DArr<unsigned int>(MeshComp::GetPrimitivesCount(), 2);
    // VBOs = (unsigned int **)oldAlloc2DArr(MeshComp::GetPrimitivesCount(), 2, 4);

    for (int i = 0; i < MeshComp::GetPrimitivesCount(); i++) {
        unsigned int EBO;

        glGenBuffers(2, VBOs[i]);
        glGenBuffers(1, &EBO);

        glGenVertexArrays(1, &VAOs[i]);
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][0]);
        glBufferData(GL_ARRAY_BUFFER, MeshComp::GetVertCount(i) * sizeof(float) * 12, MeshComp::GetVertices(i),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshComp::GetIndCount(i) * sizeof(int), MeshComp::GetIndices(i),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 10));
        glEnableVertexAttribArray(3);

        // glGenBuffers(1, &VBOs[i][1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
        // std::cout << "test1" << std::endl;
        // glEnableVertexAttribArray(4);
        // glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
        // glVertexAttribDivisor(4, 1);

        for (unsigned int j = 0; j < 4; j++) {
            glEnableVertexAttribArray(4 + j);
            glVertexAttribPointer(4 + j, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(float) * j * 4));
            glVertexAttribDivisor(4 + j, 1);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    // rc->indCounts = sp->indCounts;
    // rc->indices = sp->indices;
    // rc->primitivesCount = sp->primitivesCount;
    // rc->modelMats = sp->modelMats;
    // rc->mat = sc->mat;
    // rc->materials = sp->materials;
    // sc->rc = rc;
}

void InstanceRenderer::RenderInstanced(int width, int height)
{
    // Scene
    glViewport(0, 0, width, height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = CameraComp::GetViewMat();

    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f, (float)width / (float)height, 0.1f, 2000000.0f);

    for (int i = 0; i < MeshComp::GetPrimitivesCount(); i++) {
        int material = MeshComp::GetMaterial(i);
        glUseProgram(Materials::GetShader(material));
        UniformVec3v(Materials::GetShader(material), "viewPos", CameraComp::GetCameraPos());
        UniformVec3(Materials::GetShader(material), "light.direction", 0.4, -1.0, -0.4);
        UniformVec3(Materials::GetShader(material), "light.color", 3.0f, 3.0f, 3.0f);

        // CubeMVPuniforms
        UniformMat4v(Materials::GetShader(material), "lightSpaceMatrix", lightSpaceMatrix);
        // UniformMat4v(sc->mat->shaders[materia);
        UniformMat4v(Materials::GetShader(material), "view", view);
        UniformMat4v(Materials::GetShader(material), "projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Materials::GetDepthMap());
        for (unsigned int j = 1; j < Materials::GetTextureCount(material); j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            // NT_INTER_WARN("{}, {}", j, Materials::GetTexture(material, j));
            glBindTexture(GL_TEXTURE_2D, Materials::GetTexture(material, j));
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
        if (MeshComp::ShouldUpdate(i)) {
            // glm::mat4 test = MeshComp::GetModelMats(i)[0];
            // NT_INTER_INFO(glm::to_string(test * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MeshComp::GetInstanceCount(i), MeshComp::GetModelMats(i),
                         GL_DYNAMIC_DRAW);
            // for(int j = 0; j < MeshComp::mInstanceCount.at(i); j++) {
            // if(MeshComp::mUpdate.at(i).at(j + 1)) {
            ////UniformMat4v(sc->mat->shaders[material], "model",
            /// MeshComp::mModelMats.at(i).at(j + 1));
            // MeshComp::UpdateDone(i, j);
            // }
            // }
        }
        // glm::mat4 positions[] = {glm::translate(glm::mat4(1.0f), {0.0f, 0.0f,
        // 0.0f}), glm::translate(glm::mat4(1.0f), {0.0f, 1.0f, 3.0f}),
        // glm::translate(glm::mat4(1.0f), {0.0f, 2.0f, 6.0f})};

        glBindVertexArray(VAOs[i]);
        glDrawElementsInstanced(GL_TRIANGLES, MeshComp::GetIndCount(i), GL_UNSIGNED_INT, 0,
                                MeshComp::GetInstanceCount(i));
        for (unsigned int j = 0; j < Materials::GetTextureCount(material); j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void InstanceRenderer::RenderInstancedShadows(int shaderProgram)
{
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);

    glm::vec3 pos = { -1.6f, 4.0f, 1.6f };
    glm::vec3 target = { 0.0f, 0.0f, 0.0f };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    glm::mat4 lightView = glm::lookAt(pos, target, up);

    lightSpaceMatrix = lightProjection * lightView;

    glViewport(0, 0, 4096, 4096);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 1; i < MeshComp::GetPrimitivesCount(); i++) {
        glUseProgram(shaderProgram);
        UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Materials::GetDepthMap());
        // for (unsigned int j = 0; j < Materials::GetTextureCount(i); j++) {
        // glActiveTexture(GL_TEXTURE0 + j);
        // glBindTexture(GL_TEXTURE_2D, Materials::GetTexture(i, j));
        //}

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
        if (MeshComp::ShouldUpdate(i)) {
            // glm::mat4 test = MeshComp::GetModelMats(i)[0];
            // NT_INTER_INFO(glm::to_string(test * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MeshComp::GetInstanceCount(i), MeshComp::GetModelMats(i),
                         GL_DYNAMIC_DRAW);
        }
        //  UniformMat4v(shaderProgram, "model", MeshComp::mModelMats[i]);
        glBindVertexArray(VAOs[i]);
        glDrawElementsInstanced(GL_TRIANGLES, MeshComp::GetIndCount(i), GL_UNSIGNED_INT, 0,
                                MeshComp::GetInstanceCount(i));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
}  // namespace neith
