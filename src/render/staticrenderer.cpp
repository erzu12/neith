#include "staticrenderer.h"

#include <iostream>

#include "shaders.h"
#include "textures.h"
#include "scene/mesh.h"
#include "scene/material.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "window/window.h"
#include "scene/components/meshComp.h"

#include "vecmath.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#define GLM_FORCE_RADIANS

#include <glad/glad.h>

namespace neith {
    StaticRenderer::StaticRenderer(struct Scene *sc, struct Window *window) {
        //StaticPrimitives *sp = sc->sp;
        //struct RenderContext *rc = (struct RenderContext*)malloc(sizeof(struct RenderContext));
        VAOs = (unsigned int*)malloc(MeshComp::mPrimitivesCount * sizeof(int));

        for(int i = 0; i < MeshComp::mPrimitivesCount; i++) {
            unsigned int VBO, EBO;

            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glGenVertexArrays(1, &VAOs[i]);
            glBindVertexArray(VAOs[i]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, MeshComp::mVertCounts[i] * sizeof(float) * 12, MeshComp::mVertices[i], GL_STATIC_DRAW);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshComp::mIndCounts[i] * sizeof(int), MeshComp::mIndices[i], GL_STATIC_DRAW);
            
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(sizeof(float) * 3));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(sizeof(float) * 6));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(sizeof(float) * 10));
            glEnableVertexAttribArray(3);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //rc->indCounts = sp->indCounts;
        //rc->indices = sp->indices;
        //rc->primitivesCount = sp->primitivesCount;
        //rc->modelMats = sp->modelMats;
        //rc->mat = sc->mat;
        //rc->materials = sp->materials;
        //sc->rc = rc;
    }

    void StaticRenderer::RenderStatic(struct Scene *sc, int width, int height) {
        //Scene
        Camera *cd = sc->cd;
        glViewport(0, 0, width, height);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 view = glm::mat4(1.0f);
        cd->CameraGetViewMat(view);
        glm::mat4 projection = glm::perspective(PI / 2.0f, (float)width / (float)height, 0.1f, 1000.0f);


        for(int i = 0; i < MeshComp::mPrimitivesCount; i++) {
            int material = MeshComp::mMaterials[i];
            glUseProgram(sc->mat->shaders[material]);
            float cameraPos[3];
            F3ToArr(cd->cameraPos, cameraPos);
            UniformVec3v(sc->mat->shaders[material], "viewPos", cameraPos);
            UniformVec3(sc->mat->shaders[material], "light.direction", 0.4, -1.0, -0.4);
            UniformVec3(sc->mat->shaders[material], "light.color", 3.0f, 3.0f, 3.0f);

            //CubeMVPuniforms
            UniformMat4v(sc->mat->shaders[material], "lightSpaceMatrix", lightSpaceMatrix);
            //UniformMat4v(sc->mat->shaders[material], "model", model[0]);
            UniformMat4v(sc->mat->shaders[material], "view", view);
            UniformMat4v(sc->mat->shaders[material], "projection", projection);

            for(int j = 0; j < sc->mat->textureCounts[material]; j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, sc->mat->textures[material][j]);
            }

            if(MeshComp::mUpdate.at(i)) {
                UniformMat4v(sc->mat->shaders[material], "model", MeshComp::mModelMats[i]);
                MeshComp::UpdateDone(i);
                //std::cout << "Update" << std::endl;
            }
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, MeshComp::mIndCounts[i], GL_UNSIGNED_INT, 0);
        }
    }

    void StaticRenderer::RenderStaticShadows(struct Scene *sc, int shaderProgram) {
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);

        glm::vec3 test = {1.6f, 4.0f, 1.6f};
        glm::vec3 test1 = {0.0f, 0.0f, 0.0f};
        glm::vec3 test2 = {0.0f, 1.0f, 0.0f};
        glm::mat4 lightView = glm::lookAt(test, test1, test2 );

        lightSpaceMatrix = lightProjection * lightView;

        glUseProgram(shaderProgram);
        UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, 4096, 4096);
        glClear(GL_DEPTH_BUFFER_BIT);

        for(int i = 0; i < MeshComp::mPrimitivesCount; i++) {
            for(int j = 0; j < sc->mat->textureCounts[i]; j++) {
                glActiveTexture(GL_TEXTURE0 + j);
                glBindTexture(GL_TEXTURE_2D, sc->mat->textures[i][j]);
            }

            UniformMat4v(shaderProgram, "model", MeshComp::mModelMats[i]);
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, MeshComp::mIndCounts[i], GL_UNSIGNED_INT, 0);
        }

    }
}
