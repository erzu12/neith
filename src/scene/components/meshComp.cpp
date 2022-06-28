#include "meshComp.h"

#include <glm/gtx/string_cast.hpp>

#include "log.h"

namespace neith {

int MeshComp::mPrimitivesCount = 0;
int MeshComp::mMeshCount = 0;
std::vector<std::vector<glm::mat4>> MeshComp::mModelMats;
std::vector<int> MeshComp::mInstanceCount;
std::vector<std::unordered_map<unsigned int, unsigned int>> MeshComp::mInstanceMap;
std::vector<std::vector<bool>> MeshComp::mUpdate;
std::vector<float *> MeshComp::mVertices;
std::vector<int *> MeshComp::mIndices;
std::vector<int> MeshComp::mMeshes;
std::vector<int> MeshComp::mVertCounts;
std::vector<int> MeshComp::mIndCounts;
std::vector<int> MeshComp::mMaterials;

int MeshComp::AddStaticPrimitive(float *vertices, int vertCount, int *indices, int indCount, int material)
{
    mVertices.push_back(vertices);
    mIndices.push_back(indices);
    mVertCounts.push_back(vertCount);
    mIndCounts.push_back(indCount);
    mMaterials.push_back(material);
    mInstanceCount.push_back(0);
    mInstanceMap.push_back(std::unordered_map<unsigned int, unsigned int>());
    mModelMats.push_back(std::vector<glm::mat4>());
    mUpdate.push_back(std::vector<bool>{ false });

    mPrimitivesCount = mModelMats.size();

    return mPrimitivesCount - 1;
}

int MeshComp::AddStaticMesh(int primitivesCount)
{
    int mesh = mMeshCount;
    mMeshCount++;

    mMeshes.push_back(mPrimitivesCount);
    mMeshes.push_back(primitivesCount);

    return mesh;
}

void MeshComp::Transform(unsigned int entityID, glm::mat4 &transform)
{
    mesh meshID = mIndexMap.at(entityID);
    for (int i = 0; i < mMeshes[meshID * 2 + 1]; i++) {
        int primitive = mMeshes[meshID + i];
        unsigned int instanceID = mInstanceMap.at(primitive).at(entityID);
        mModelMats.at(primitive).at(instanceID) *= transform;
        ScheduleUpdate(primitive, instanceID);
    }
}

void MeshComp::ScheduleUpdate(mesh meshID, unsigned int instanceID)
{
    mUpdate.at(meshID).at(instanceID + 1) = true;
    mUpdate.at(meshID).at(0) = true;
}

void MeshComp::UpdateDone(mesh meshID, unsigned int instanceID)
{
    mUpdate.at(meshID).at(instanceID + 1) = false;
    mUpdate.at(meshID).at(0) = false;
}

bool MeshComp::ShouldUpdate(mesh meshID) { return mUpdate.at(meshID).at(0); }

void MeshComp::AddInstance(unsigned int meshID, unsigned int entityID, glm::mat4 modelMat)
{
    mIndexMap.insert({ entityID, meshID });

    for (int i = 0; i < mMeshes[meshID * 2 + 1]; i++) {
        int primitive = mMeshes[meshID * 2] + i;

        mInstanceMap.at(primitive).insert({ entityID, mInstanceCount.at(primitive) });
        mModelMats.at(primitive).push_back(modelMat);
        mUpdate.at(primitive).push_back(true);
        mUpdate.at(primitive).at(0) = true;

        mInstanceCount.at(primitive)++;
    }
}

int MeshComp::GetMaterial(unsigned int meshID, int material)
{
    if (mMeshes[meshID * 2 + 1] <= material) {
        NT_INTER_ERROR("warning: mesh: {} does not have: {} materialSlots", meshID, material);
        return -1;
    }
    return mMaterials[mMeshes[meshID * 2] + material];
}

// void MeshComp::UpdateTransform(unsigned int entity, glm::mat4 transform) {
//     unsigned int i = mIndexMap.at(entity);
//     mModelMats[i] += transform;
// }

DebugInfo *MeshComp::GetDebugInfo(unsigned int entityID)
{
    unsigned int meshID = mIndexMap.at(entityID);

    DebugInfo *nameInfo = new DebugInfo("Mesh Component");
    DebugInfo *debugInfo = new DebugInfo("primitives:");

    for (int i = 0; i < mMeshes[meshID * 2 + 1]; i++) {
        int primitive = mMeshes[meshID * 2] + i;
        unsigned int instanceID = mInstanceMap.at(primitive).at(entityID);
        DebugInfo *primitiveInfo = new DebugInfo("primitive" + std::to_string(i) + ":");
        debugInfo->child.push_back(primitiveInfo);
        primitiveInfo->child.push_back(new DebugInfo("vertices: " + std::to_string(mVertCounts.at(primitive))));
        primitiveInfo->child.push_back(new DebugInfo("indices: " + std::to_string(mIndCounts.at(primitive))));
        primitiveInfo->child.push_back(new DebugInfo("material: " + std::to_string(mMaterials.at(primitive))));
        primitiveInfo->child.push_back(
            new DebugInfo("Model Matrix: " + glm::to_string(mModelMats.at(primitive).at(instanceID))));
    }
    nameInfo->child.push_back(debugInfo);
    return nameInfo;
}

}  // namespace neith
