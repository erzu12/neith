#include "staticrenderer.h"

#include "shaders.h"
#include "textures.h"
#include "scene/mesh.h"
#include "scene/material.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "window/window.h"

#include <glad/glad.h>

StaticRenderer::StaticRenderer(struct Scene *sc, struct Window *window) {
    StaticPrimitives *sp = sc->sp;
    //struct RenderContext *rc = (struct RenderContext*)malloc(sizeof(struct RenderContext));
    VAOs = (unsigned int*)malloc(sp->primitivesCount * sizeof(int));

    for(int i = 0; i < sp->primitivesCount; i++) {
        unsigned int VBO, EBO;

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glGenVertexArrays(1, &VAOs[i]);
        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sp->vertCounts[i] * sizeof(float) * 12, sp->vertices[i], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sp->indCounts[i] * sizeof(int), sp->indices[i], GL_STATIC_DRAW);
        
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

    
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    cd->CameraGetViewMat(view);
    /*
    printf("\n{");
    for(int i = 0; i < 4; i++) {
        printf("%f, %f, %f, %f\n", view[i][0], view[i][1], view[i][2], view[i][3]);
    }
    printf("}\n");
    */   
    mat4 projection;
    glm_perspective(GLM_PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f, projection);


    for(int i = 0; i < sc->sp->primitivesCount; i++) {
        int material = sc->sp->materials[i];
        glUseProgram(sc->mat->shaders[material]);
        float cameraPos[3];
        F3ToArr(cd->cameraPos, cameraPos);
        UniformVec3v(sc->mat->shaders[material], "viewPos", cameraPos);
        UniformVec3(sc->mat->shaders[material], "light.direction", 0.4, -1.0, -0.4);
        UniformVec3(sc->mat->shaders[material], "light.color", 3.0f, 3.0f, 3.0f);

        //CubeMVPuniforms
        UniformMat4v(sc->mat->shaders[material], "lightSpaceMatrix", lightSpaceMatrix[0]);
        //UniformMat4v(sc->mat->shaders[material], "model", model[0]);
        UniformMat4v(sc->mat->shaders[material], "view", view[0]);
        UniformMat4v(sc->mat->shaders[material], "projection", projection[0]);

        for(int j = 0; j < sc->mat->textureCounts[material]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, sc->mat->textures[material][j]);
        }

		UniformMat4v(sc->mat->shaders[material], "model", sc->sp->modelMats[i][0]);
		glBindVertexArray(VAOs[i]);
		glDrawElements(GL_TRIANGLES, sc->sp->indCounts[i], GL_UNSIGNED_INT, 0);
    }
}

void StaticRenderer::RenderStaticShadows(struct Scene *sc, int shaderProgram) {
    mat4 lightProjection;
    glm_ortho(-10.0, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f, lightProjection);

    mat4 lightView = GLM_MAT4_ZERO_INIT;
    vec3 test = {1.6f, 4.0f, 1.6f};
    vec3 test1 = {0.0f, 0.0f, 0.0f};
    vec3 test2 = {0.0f, 1.0f, 0.0f};
    glm_lookat(test, test1,
           test2, lightView);

    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

    glUseProgram(shaderProgram);
    UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);
    glViewport(0, 0, 4096, 4096);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < sc->sp->primitivesCount; i++) {
        for(int j = 0; j < sc->mat->textureCounts[i]; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, sc->mat->textures[i][j]);
        }

		UniformMat4v(shaderProgram, "model", sc->sp->modelMats[i][0]);
		glBindVertexArray(VAOs[i]);
		glDrawElements(GL_TRIANGLES, sc->sp->indCounts[i], GL_UNSIGNED_INT, 0);
    }

}

