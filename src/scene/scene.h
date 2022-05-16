#pragma once

#include <glm/vec3.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "entity.h"
#include "material.h"
#include "mesh.h"

// struct RenderContext {
//    struct Materials *mat;
//    struct Window *win;
//    int *materials;
//    int primitivesCount;
//    mat4 *modelMats;
//    mat4 lightSpaceMatrix;
//    unsigned int *VAOs;
//    unsigned int texColorBuffer;
//    unsigned int screenTexture;
//    unsigned int screenShader;
//    unsigned int FBO;
//    unsigned int depthMapFBO;
//    unsigned int intermediateFBO;
//    unsigned int shadowMapShader;
//    unsigned int texColorBuffer;
//    unsigned int screenTexture;
//    unsigned int cubeMap;
//    unsigned int screenVAO;
//    unsigned int cubeMapVAO;
//    unsigned int cubeMapShader;
//    int *indCounts;
//    int **indices;
// };

namespace neith {
class Scene {
private:
    static Materials *mMaterial;
    glm::vec3 lightDir;

public:
    Scene();

    static Entity *GetEntity(unsigned int entityID);

    glm::vec3 GetLightDir();

    static void Save(std::ofstream file);

    ~Scene();

    // StaticPrimitives *sp;
    // struct RenderContext *rc;
};
}  // namespace neith
