#include "scene/ecsmanager.h"
#include "instancerenderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "dataStructures.h"
#include "scene/components/cameraComp.h"
#include "scene/material.h"
#include "scene/mesh.h"
#include "scene/rendercontext.h"
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
}

void InstanceRenderer::renderSystem(int width, int height, unsigned int depthMap) {
    glViewport(0, 0, width, height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = CameraComp::GetViewMat();

    glm::mat4 projection = glm::perspective(glm::pi<float>() / 2.0f, (float)width / (float)height, 0.1f, 2000000.0f);

    ECSManager::ecs.query<PrimitiveRenderContext>()->each([&](PrimitiveRenderContext& prc) {
        //unsigned int primitiveID = MeshComp::GetPrimitivesID(mesh, LOD, primitive);
        
        //int material = MeshComp::GetMaterial(primitiveID);
        Shader *shader = prc.material->getShader();
        glUseProgram(shader->mShaderProgram);
        shader->UniformVec3v("viewPos", CameraComp::GetCameraPos());
        shader->UniformVec3("light.direction", 0.4, -1.0, -0.4);
        shader->UniformVec3("light.color", 3.0f, 3.0f, 3.0f);

        // CubeMVPuniforms
        shader->UniformMat4v("lightSpaceMatrix", mLightSpaceMatrix);
        shader->UniformMat4v("view", view);
        shader->UniformMat4v("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // for (unsigned int j = 1; j < Materials::GetTextureCount(material); j++) {
        for (unsigned int j = 1; j < 16; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            // NT_INTER_WARN("{}, {}", j, Materials::GetTexture(material, j));
            glBindTexture(GL_TEXTURE_2D, prc.material->getTexture(j));
        }

        //if (Materials::IsBackfaced(material)) {
            //glDisable(GL_CULL_FACE);
        //}

        glBindVertexArray(prc.mVAO);
        glDrawElementsInstanced(GL_TRIANGLES, prc.indCount, GL_UNSIGNED_INT, 0,
                prc.instanceCount);
        for (unsigned int j = 0; j < 16; j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glEnable(GL_CULL_FACE);
    });
}



//void InstanceRenderer::RenderInstancedShadows(int shaderProgram, unsigned int depthMap)
//{
    // glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);

    // glm::vec3 pos = { -1.6f, 4.0f, 1.6f };
    // glm::vec3 target = { 0.0f, 0.0f, 0.0f };
    // glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    // glm::mat4 lightView = glm::lookAt(pos, target, up);

    // mLightSpaceMatrix = lightProjection * lightView;

    // glViewport(0, 0, 4096, 4096);
    // glClear(GL_DEPTH_BUFFER_BIT);

    // for (int i = 1; i < MeshComp::GetPrimitivesCount(); i++) {
    // glUseProgram(shaderProgram);
    // UniformMat4v(shaderProgram, "lightSpaceMatrix", mLightSpaceMatrix);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, depthMap);
    // glActiveTexture(GL_TEXTURE1);
    // int material = MeshComp::GetMaterial(i);
    // glBindTexture(GL_TEXTURE_2D, Materials::GetTransparancyTexture(material));
    //// for (unsigned int j = 0; j < Materials::GetTextureCount(i); j++) {
    //// glActiveTexture(GL_TEXTURE0 + j);
    //// glBindTexture(GL_TEXTURE_2D, Materials::GetTexture(i, j));
    ////}

    // glBindBuffer(GL_ARRAY_BUFFER, VBOs[i][1]);
    // if (MeshComp::ShouldUpdate(i)) {
    //// glm::mat4 test = MeshComp::GetModelMats(i)[0];
    //// NT_INTER_INFO(glm::to_string(test * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MeshComp::GetInstanceCount(i), MeshComp::GetModelMats(i),
    // GL_DYNAMIC_DRAW);
    //}
    ////  UniformMat4v(shaderProgram, "model", MeshComp::mModelMats[i]);
    // glBindVertexArray(VAOs[i]);
    // glDrawElementsInstanced(GL_TRIANGLES, MeshComp::GetIndCount(i), GL_UNSIGNED_INT, 0,
    // MeshComp::GetInstanceCount(i));
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, 0);
    //}
//}
}  // namespace neith
