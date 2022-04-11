#pragma once

#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "component.h"

typedef unsigned int mesh;

namespace neith {

class MeshComp : public Component {
public:
    static int AddStaticPrimitive(float *vertices, int vertCount, int *indices, int indCount, int material);
    static void Transform(unsigned int entityID, glm::mat4 &transform);
    static void ShouldUpdate(mesh meshID, unsigned int instanceID);
    static void UpdateDone(mesh meshID, unsigned int instanceID);
    static void AddInstance(unsigned int meshID, unsigned int entityID, glm::mat4 modelMat);
    static int AddStaticMesh(int primitivesCount);
    static int getMaterial(unsigned int entity, int material);

    static int GetPrimitivesCount() { return mPrimitivesCount; }

    static int GetVertCount(int i);
    static int GetIndCount(int i);
    static float *GetVertices(int i);
    static int *GetIndices(int i);

private:
    static int id;
    static int mPrimitivesCount;
    static int mMeshCount;
    static std::vector<std::vector<glm::mat4>> mModelMats;
    static std::vector<int> mInstanceCount;
    static std::vector<std::unordered_map<unsigned int, unsigned int>> mInstanceMap;
    static std::vector<std::vector<bool>> mUpdate;
    static std::vector<float *> mVertices;
    static std::vector<int *> mIndices;
    static std::vector<int> mMeshes;
    static std::vector<int> mVertCounts;
    static std::vector<int> mIndCounts;
    static std::vector<int> mMaterials;

    // void static UpdateTransform(unsigned int entity, glm::mat4 transform);

    //~StaticPrimitives();
};
}  // namespace neith
