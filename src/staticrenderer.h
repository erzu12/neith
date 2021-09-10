#pragma once

#include "mesh.h"
#include "shaders.h"

#include <glad/glad.h>

struct RenderContext {
    int primitivesCount;
    mat4 *modelMats;
    unsigned int *VAOs;
    int *indCounts;
    int *indOffsets;
};

struct RenderContext *InitStaticRender(struct StaticPrimitives *sp) {
    struct RenderContext *rc = (struct RenderContext*)malloc(sizeof(struct RenderContext));
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
    return rc;
}

void RenderStatic(struct RenderContext *rc, int shaderProgram) {
    for(int i = 0; i < rc->primitivesCount; i++) {

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

