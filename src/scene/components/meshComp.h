#pragma once

#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "component.h"
#include "debugInfo.h"

typedef unsigned int mesh;

namespace neith {

class MeshComp : public Component {
public:
    static MeshComp *GetInstance()
    {
        static MeshComp instance;
        return &instance;
    }
    static int AddStaticPrimitive(float *vertices, int vertCount, int *indices, int indCount, int material);
    static bool ShouldUpdate(mesh meshID);
    static void ScheduleUpdate(mesh meshID, unsigned int instanceID);
    static void UpdateDone(mesh meshID, unsigned int instanceID);
    static void AddInstance(unsigned int meshID, unsigned int entityID, glm::mat4 modelMat);
    static int AddStaticMesh(int primitivesCount);

    static int GetPrimitivesCount() { return mPrimitivesCount; }
    static int GetInstanceCount(mesh meshID) { return mInstanceCount.at(meshID); }
    static int GetVertCount(unsigned int primitveID) { return mVertCounts.at(primitveID); }
    static int GetIndCount(unsigned int primitveID) { return mIndCounts.at(primitveID); }
    static float *GetVertices(unsigned int primitveID) { return mVertices.at(primitveID); }
    static int *GetIndices(unsigned int primitveID) { return mIndices.at(primitveID); }
    static int GetMaterial(unsigned int primitveID) { return mMaterials.at(primitveID); }
    static int GetMaterial(unsigned int entity, int material);
    static glm::mat4 *GetModelMats(unsigned int primitveID) { return mModelMats.at(primitveID).data(); }

    void Transform(unsigned int entityID, glm::mat4 &transform) override;
    DebugInfo *GetDebugInfo(unsigned int entityID) override;

private:
    MeshComp(){};
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
public:
    MeshComp(MeshComp &other) = delete;
    void operator=(const MeshComp &) = delete;
};
}  // namespace neith
