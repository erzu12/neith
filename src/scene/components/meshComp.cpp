#include "meshComp.h"

#include <glm/gtx/string_cast.hpp>

#include "log.h"

namespace neith {

int MeshComp::mPrimitivesCount = 0;
int MeshComp::mMeshCount = 0;
std::vector<std::vector<glm::mat4>> MeshComp::mModelMats;
// mesh<LOD<instance<modelMat>>>
std::vector<std::vector<std::vector<glm::mat4>>> MeshComp::mLODModelMats;
std::mutex MeshComp::mLODModelMatsMutex;
std::vector<int> MeshComp::mInstanceCount;
// primitive<entityID, instanceID>
std::vector<std::unordered_map<unsigned int, unsigned int>> MeshComp::mInstanceMap;
std::vector<std::vector<bool>> MeshComp::mUpdate;
std::vector<float *> MeshComp::mVertices;
std::vector<int *> MeshComp::mIndices;
// std::vector<int> MeshComp::mMeshes;
// Mesh<LOD<PrimitivesID>>
std::vector<std::vector<std::vector<unsigned int>>> MeshComp::mMeshes;
std::vector<float *> MeshComp::mLODDistances;
std::vector<int> MeshComp::mVertCounts;
std::vector<int> MeshComp::mIndCounts;
std::vector<int> MeshComp::mMaterials;

int MeshComp::AddStaticPrimitive(
    float *vertices, int vertCount, int *indices, int indCount, unsigned int mesh, unsigned int LOD, int material)
{
    mMeshes.at(mesh).at(LOD).push_back(mPrimitivesCount);
    mVertices.push_back(vertices);
    mIndices.push_back(indices);
    mVertCounts.push_back(vertCount);
    mIndCounts.push_back(indCount);
    mMaterials.push_back(material);
    // mModelMats.push_back(std::vector<glm::mat4>());
    mUpdate.push_back(std::vector<bool>{ false });

    mPrimitivesCount = mMaterials.size();

    return mPrimitivesCount - 1;
}

int MeshComp::AddStaticMesh(int primitivesCount)
{
    mInstanceMap.push_back(std::unordered_map<unsigned int, unsigned int>());
    mLODModelMats.push_back(std::vector<std::vector<glm::mat4>>());
    mInstanceCount.push_back(0);
    mLODDistances.push_back(nullptr);
    int mesh = mMeshCount;
    mMeshCount++;

    mMeshes.push_back(std::vector<std::vector<unsigned int>>());

    return mesh;
}

void MeshComp::SetLODs(unsigned int meshID, int LODCount, float *LODDistances)
{
    mLODDistances.at(meshID) = LODDistances;
    mMeshes.at(meshID).insert(mMeshes.at(meshID).begin(), LODCount + 1, std::vector<unsigned int>());
    mLODModelMats.at(meshID).insert(mLODModelMats.at(meshID).begin(), LODCount + 1, std::vector<glm::mat4>());
}

// yea dis needs some work
void MeshComp::UpdateLODs(glm::vec3 camPos)
{
    for (int mesh = 0; mesh < mMeshes.size(); mesh++) {
        for (int LOD = mMeshes.at(mesh).size() - 1; LOD >= 0; LOD--) {
            for (int instance = 0; instance < mLODModelMats.at(mesh).at(LOD).size(); instance++) {
                glm::mat4 modelMat = mLODModelMats.at(mesh).at(LOD).at(instance);
                float x = modelMat[3][0] - camPos.x;
                float y = modelMat[3][1] - camPos.y;
                float z = modelMat[3][2] - camPos.z;
                float dist = x * x + y * y + z * z;
                // the stupid way
                if (LOD == mMeshes.at(mesh).size() - 1) {
                    if (dist < mLODDistances.at(mesh)[LOD - 1]) {
                        mLODModelMatsMutex.lock();
                        mLODModelMats.at(mesh).at(LOD - 1).push_back(mLODModelMats.at(mesh).at(LOD).at(instance));
                        glm::mat4 *mats = mLODModelMats.at(mesh).at(LOD).data();
                        glm::mat4 lastMat = mats[mLODModelMats.at(mesh).at(LOD).size()];
                        mats[instance] = lastMat;
                        mLODModelMats.at(mesh).at(LOD).pop_back();
                        mLODModelMatsMutex.unlock();
                        instance--;
                    }
                }
                else if (LOD == 0) {
                    if (dist > mLODDistances.at(mesh)[LOD]) {
                        mLODModelMatsMutex.lock();
                        mLODModelMats.at(mesh).at(LOD + 1).push_back(mLODModelMats.at(mesh).at(LOD).at(instance));
                        glm::mat4 *mats = mLODModelMats.at(mesh).at(LOD).data();
                        glm::mat4 lastMat = mats[mLODModelMats.at(mesh).at(LOD).size()];
                        mats[instance] = lastMat;
                        mLODModelMats.at(mesh).at(LOD).pop_back();
                        mLODModelMatsMutex.unlock();
                        instance--;
                    }
                }

                else if (dist > mLODDistances.at(mesh)[LOD]) {
                    mLODModelMatsMutex.lock();
                    mLODModelMats.at(mesh).at(LOD + 1).push_back(mLODModelMats.at(mesh).at(LOD).at(instance));
                    glm::mat4 *mats = mLODModelMats.at(mesh).at(LOD).data();
                    glm::mat4 lastMat = mats[mLODModelMats.at(mesh).at(LOD).size()];
                    mats[instance] = lastMat;
                    mLODModelMats.at(mesh).at(LOD).pop_back();
                    mLODModelMatsMutex.unlock();
                    instance--;
                }
                else if (dist < mLODDistances.at(mesh)[LOD - 1]) {
                    mLODModelMatsMutex.lock();
                    mLODModelMats.at(mesh).at(LOD - 1).push_back(mLODModelMats.at(mesh).at(LOD).at(instance));
                    glm::mat4 *mats = mLODModelMats.at(mesh).at(LOD).data();
                    glm::mat4 lastMat = mats[mLODModelMats.at(mesh).at(LOD).size()];
                    mats[instance] = lastMat;
                    mLODModelMats.at(mesh).at(LOD).pop_back();
                    mLODModelMatsMutex.unlock();
                    instance--;
                }
            }
        }
    }
}

void MeshComp::Transform(unsigned int entityID, glm::mat4 &transform)
{
    // mesh meshID = mIndexMap.at(entityID);
    // for (int i = 0; i < mMeshes[meshID * 2 + 1]; i++) {
    // int primitive = mMeshes[meshID + i];
    // unsigned int instanceID = mInstanceMap.at(primitive).at(entityID);
    // mModelMats.at(primitive).at(instanceID) *= transform;
    // ScheduleUpdate(primitive, instanceID);
    //}
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
    mInstanceMap.at(meshID).insert({ entityID, mInstanceCount.at(meshID) });
    mInstanceCount.at(meshID)++;
    mLODModelMats.at(meshID).at(0).push_back(modelMat);

    for (std::vector<unsigned int> LOD : mMeshes.at(meshID)) {
        for (unsigned int primitive : LOD) {
            // mModelMats.at(primitive).push_back(modelMat);
            mUpdate.at(primitive).push_back(true);
            mUpdate.at(primitive).at(0) = true;
        }
    }
}

int MeshComp::GetMaterial(unsigned int meshID, int material)
{
    if (mMeshes.at(meshID).at(0).size() <= material) {
        NT_INTER_ERROR("warning: mesh: {} does not have: {} materialSlots", meshID, material);
        return -1;
    }
    return mMaterials.at(mMeshes.at(meshID).at(0).at(material));
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

    for (std::vector<unsigned int> LOD : mMeshes.at(meshID)) {
        for (unsigned int primitive : LOD) {
            unsigned int instanceID = mInstanceMap.at(primitive).at(entityID);
            DebugInfo *primitiveInfo = new DebugInfo("primitive " + std::to_string(primitive) + ":");
            debugInfo->child.push_back(primitiveInfo);
            primitiveInfo->child.push_back(new DebugInfo("vertices: " + std::to_string(mVertCounts.at(primitive))));
            primitiveInfo->child.push_back(new DebugInfo("indices: " + std::to_string(mIndCounts.at(primitive))));
            primitiveInfo->child.push_back(new DebugInfo("material: " + std::to_string(mMaterials.at(primitive))));
            primitiveInfo->child.push_back(
                new DebugInfo("Model Matrix: " + glm::to_string(mModelMats.at(primitive).at(instanceID))));
        }
    }
    nameInfo->child.push_back(debugInfo);
    return nameInfo;
}

}  // namespace neith
