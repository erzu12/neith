#include "instancerenderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "dataStructures.h"
#include "scene/camera.h"
#include "scene/components/meshComp.h"
#include "scene/material.h"
#include "scene/mesh.h"
#include "scene/scene.h"
#include "shaders.h"
#include "textures.h"
#include "vecmath.h"
#include "window/window.h"
#define GLM_FORCE_RADIANS

#include <glad/glad.h>

namespace neith {

InstanceRenderer::InstanceRenderer(struct Scene *sc, struct Window *window)
{
    // StaticPrimitives *sp = sc->sp;
    // struct RenderContext *rc = (struct RenderContext*)malloc(sizeof(struct
    // RenderContext));
    VAOs = (unsigned int *)malloc(MeshComp::mPrimitivesCount * sizeof(int));
    VBOs = (unsigned int **)Alloc2DArr(MeshComp::mPrimitivesCount, 2, 4);

    for (int i = 0; i < MeshComp::mPrimitivesCount; i++) {
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

void InstanceRenderer::RenderInstanced(struct Scene *sc, int width, int height)
{
    // Scene
    Camera *cd = sc->GetCamera();
    glViewport(0, 0, width, height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::mat4(1.0f);
    cd->CameraGetViewMat(view);
    glm::mat4 projection = glm::perspective(PI / 2.0f, (float)width / (float)height, 0.1f, 2000000.0f);

    for (int i = 0; i < MeshComp::mPrimitivesCount; i++) {
        int material = MeshComp::mMaterials[i];
        glUseProgram(Materials::mShaders[material]);
        float cameraPos[3];
        F3ToArr(cd->mCameraPos, cameraPos);
        UniformVec3v(Materials::mShaders[material], "viewPos", cameraPos);
        UniformVec3(Materials::mShaders[material], "light.direction", 0.4, -1.0, -0.4);
        UniformVec3(Materials::mShaders[material], "light.color", 3.0f, 3.0f, 3.0f);

        // CubeMVPuniforms
        UniformMat4v(Materials::mShaders[material], "lightSpaceMatrix", lightSpaceMatrix);
        // UniformMat4v(sc->mat->shaders[materia);
        UniformMat4v(Materials::mShaders[material], "view", view);
        UniformMat4v(Materials::mShaders[material], "projection", projection);

        for (int j = 0; j < Materials::mTextureCounts[material]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, Materials::mTextures[material][j]);
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
        if (MeshComp::mUpdate.at(i).at(0)) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MeshComp::mInstanceCount.at(i),
                         MeshComp::mModelMats.at(i).data(), GL_DYNAMIC_DRAW);
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
        glDrawElementsInstanced(GL_TRIANGLES, MeshComp::mIndCounts[i], GL_UNSIGNED_INT, 0,
                                MeshComp::mInstanceCount.at(i));
    }
}

void InstanceRenderer::RenderInstancedShadows(struct Scene *sc, int shaderProgram)
{
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);

    glm::vec3 test = { 1.6f, 4.0f, 1.6f };
    glm::vec3 test1 = { 0.0f, 0.0f, 0.0f };
    glm::vec3 test2 = { 0.0f, 1.0f, 0.0f };
    glm::mat4 lightView = glm::lookAt(test, test1, test2);

    lightSpaceMatrix = lightProjection * lightView;

    glUseProgram(shaderProgram);
    UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, 4096, 4096);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < MeshComp::mPrimitivesCount; i++) {
        for (int j = 0; j < Materials::mTextureCounts[i]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, Materials::mTextures[i][j]);
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
        // UniformMat4v(shaderProgram, "model", MeshComp::mModelMats[i]);
        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, MeshComp::mIndCounts[i], GL_UNSIGNED_INT, 0);
    }
}
}  // namespace neith
