#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <unordered_map>
#include <string>

#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "entity.h"

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
        static std::unordered_map<std::string, unsigned int> mEntityNames;
        static std::vector<Entity*> mEntitys;
    public:
        Materials *mat;
        Camera *cd;
        glm::vec3 lightDir;

        Scene(int maxPrimitives);

        static int AddEntity();
        static int AddEntity(unsigned int parentID);
        static int AddEntity(std::string &name);
        static int AddEntity(std::string &name, unsigned int parentID);
        static int AddEntity(std::string &name, std::string parentName);

        static Entity* GetEntity(unsigned int entityID);

        ~Scene();

        //StaticPrimitives *sp;
        //struct RenderContext *rc;
    };
}
