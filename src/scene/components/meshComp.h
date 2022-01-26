#pragma once

#include <unordered_map>
#include <vector>

#include <glm/mat4x4.hpp>

#include "component.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace neith {

    class MeshComp : public Component {
public:
        static int mPrimitivesCount;
        static int mMeshCount;
        static std::vector<glm::mat4> mModelMats;
        static std::vector<bool> mUpdate;
        static std::vector<float*> mVertices;
        static std::vector<int*> mIndices;
        static std::vector<int> mMeshes;
        static std::vector<int> mVertCounts;
        static std::vector<int> mIndCounts;
        static std::vector<int> mMaterials;

        static int id;

        int static AddStaticPrimitive(unsigned int entity,
                               glm::mat4 modelMat,
                               float *vertices,
                               int vertCount,
                               int *indices,
                               int indCount,
                               int material);

        void static Transform(unsigned int entityID, glm::mat4 &transform) {
            unsigned int meshID = mIndexMap.at(entityID);
            mModelMats.at(meshID) *= transform;
            //std::cout << glm::to_string(mModelMats.at(meshID)) << std::endl;
            ShouldUpdate(meshID);
        }

        void static ShouldUpdate(unsigned int meshID) {
            mUpdate.at(meshID) = true;
        }

        void static UpdateDone(unsigned int meshID) {
            mUpdate.at(meshID) = false;
        }

        int AddStaticMesh(unsigned int entity, int primitivesCount);

        //void static UpdateTransform(unsigned int entity, glm::mat4 transform);

        //~StaticPrimitives();
    };
}
