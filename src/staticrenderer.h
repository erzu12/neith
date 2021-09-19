#pragma once

#include "mesh.h"
#include "shaders.h"
#include "material.h"
#include "scene.h"
#include "camera.h"

#include <glad/glad.h>


void InitStaticRender(struct Scene *sc) {
    struct StaticPrimitives *sp = sc->sp;
    struct RenderContext *rc = malloc(sizeof(struct RenderContext));
    rc->VAOs = malloc(sp->primitivesCount * sizeof(int));
    unsigned int VBO;
    unsigned int EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sp->vertSize, sp->vertData, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sp->indSize, sp->indData, GL_STATIC_DRAW);

    for(int i = 0; i < sp->primitivesCount; i++) {
        glGenVertexArrays(1, &rc->VAOs[i]);
        glBindVertexArray(rc->VAOs[i]);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(long)(sp->vertOffsets[i]));
        glEnableVertexAttribArray(0);

        int normalOffset = sp->vertOffsets[i] + sp->vertCounts[i] * 3 * sizeof(float);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(long)(normalOffset));
        glEnableVertexAttribArray(1);
        
        int tangentOffset = normalOffset + sp->vertCounts[i] * 3 * sizeof(float);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(long)(tangentOffset));
        glEnableVertexAttribArray(2);

        int texCoordOffset = tangentOffset + sp->vertCounts[i] * 4 * sizeof(float);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(long)texCoordOffset);
        glEnableVertexAttribArray(3);
    }
    rc->indCounts = sp->indCounts;
    rc->indOffsets = sp->indOffsets;
    rc->primitivesCount = sp->primitivesCount;
    rc->modelMats = sp->modelMats;
    rc->mat = sc->mat;
    rc->materials = sp->materials;
    sc->rc = rc;
}

void RenderStatic(struct Scene *sc, int width, int height) {
    //Scene
    struct RenderContext *rc = sc->rc;
    struct CameraData *cd = sc->cd;
    glViewport(0, 0, width, height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    CameraGetViewMat(cd, view);
        
    mat4 projection;
    glm_perspective(GLM_PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f, projection);


    for(int i = 0; i < rc->primitivesCount; i++) {
        int material = rc->materials[i];
        glUseProgram(rc->mat->shaders[material]);
        UniformVec3v(rc->mat->shaders[material], "viewPos", cd->cameraPos);
        UniformVec3(rc->mat->shaders[material], "light.direction", 0.4, -1.0, -0.4);
        UniformVec3(rc->mat->shaders[material], "light.color", 3.0f, 3.0f, 3.0f);

        //CubeMVPuniforms
        UniformMat4v(rc->mat->shaders[material], "lightSpaceMatrix", rc->lightSpaceMatrix[0]);
        //UniformMat4v(rc->mat->shaders[material], "model", model[0]);
        UniformMat4v(rc->mat->shaders[material], "view", view[0]);
        UniformMat4v(rc->mat->shaders[material], "projection", projection[0]);

        for(int j = 0; j < rc->mat->textureCounts[material]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, rc->mat->textures[material][j]);
        }

		UniformMat4v(rc->mat->shaders[material], "model", rc->modelMats[i][0]);
		glBindVertexArray(rc->VAOs[i]);
		glDrawElements(GL_TRIANGLES, rc->indCounts[i], GL_UNSIGNED_INT, (void*)(long)rc->indOffsets[i]);
    }
}

void RenderStaticShadows(struct Scene *sc, int shaderProgram) {
    struct RenderContext *rc = sc->rc;
    mat4 lightProjection;
    glm_ortho(-10.0, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f, lightProjection);

    mat4 lightView = GLM_MAT4_ZERO_INIT;
    glm_lookat((vec3){-1.6f, 4.0f, 1.6f}, (vec3){0.0f, 0.0f, 0.0f},
           (vec3){0.0f, 1.0f, 0.0f}, lightView);

    mat4 lightSpaceMatrix;
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    glUseProgram(shaderProgram);
    UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);
    glm_mat4_copy(lightSpaceMatrix, rc->lightSpaceMatrix);
    glViewport(0, 0, 4096, 4096);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < rc->primitivesCount; i++) {
        for(int j = 0; j < rc->mat->textureCounts[i]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, rc->mat->textures[i][j]);
        }

		UniformMat4v(shaderProgram, "model", rc->modelMats[i][0]);
		glBindVertexArray(rc->VAOs[i]);
		glDrawElements(GL_TRIANGLES, rc->indCounts[i], GL_UNSIGNED_INT, (void*)(long)rc->indOffsets[i]);
    }

}

void DeleteStaticRender(struct RenderContext *rc, struct StaticPrimitives *sp) {
    DeleteStaticPrimitives(sp);
    free(rc->VAOs);
    free(rc);
};

