#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <unordered_map>
#include <string>

#include "components/component.h"
#include "components/meshComp.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"

//struct RenderContext {
//   struct Materials *mat;
//   struct Window *win;
//   int *materials;
//   int primitivesCount;
//   mat4 *modelMats;
//   mat4 lightSpaceMatrix;
//   unsigned int *VAOs;
//   unsigned int texColorBuffer;
//   unsigned int screenTexture;
//   unsigned int screenShader;
//   unsigned int FBO;
//   unsigned int depthMapFBO;
//   unsigned int intermediateFBO;
//   unsigned int shadowMapShader;
//   unsigned int texColorBuffer;
//   unsigned int screenTexture;
//   unsigned int cubeMap;
//   unsigned int screenVAO;
//   unsigned int cubeMapVAO;
//   unsigned int cubeMapShader;
//   int *indCounts;
//   int **indices;
//};


namespace neith {
    class Scene {
    private:
        std::unordered_map<std::string, int> entityNames;
    public:
        StaticPrimitives *sp;
        Materials *mat;
        //struct RenderContext *rc;
        Camera *cd;
        glm::vec3 lightDir;

        Scene(int maxPrimitives);

        ~Scene();
    };
}
