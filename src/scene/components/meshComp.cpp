#include "meshComp.h"

namespace neith {

    int MeshComp::mPrimitivesCount = 0;
    int MeshComp::mMeshCount = 0;
    std::vector<glm::mat4> MeshComp::mModelMats;
    std::vector<bool> MeshComp::mUpdate;
    std::vector<float*> MeshComp::mVertices;
    std::vector<int*> MeshComp::mIndices;
    std::vector<int> MeshComp::mMeshes;
    std::vector<int> MeshComp::mVertCounts;
    std::vector<int> MeshComp::mIndCounts;
    std::vector<int> MeshComp::mMaterials;

    int MeshComp::AddStaticPrimitive(unsigned int entity,
                                             glm::mat4 modelMat,
                                             float *vertices,
                                             int vertCount,
                                             int *indices,
                                             int indCount,
                                             int material)
    {
        mModelMats.push_back(modelMat);
        mVertices.push_back(vertices);
        mIndices.push_back(indices);
        mVertCounts.push_back(vertCount);
        mIndCounts.push_back(indCount);
        mMaterials.push_back(material);
        mUpdate.push_back(true);

        mPrimitivesCount = mModelMats.size();
        mIndexMap.insert({entity, mPrimitivesCount});
            

        return mPrimitivesCount;
    }

    int MeshComp::AddStaticMesh(unsigned int entity, int primitivesCount) {
        int mesh = mMeshCount;
        mMeshCount++;

        int primitive = primitivesCount;
        primitivesCount = primitive + primitivesCount;

        //this gona segfault so hard
        mMeshes[mesh * 3] = primitivesCount;
        mMeshes[mesh * 3 + 1] = 0;
        mMeshes[mesh * 3 + 2] = primitivesCount + primitivesCount;

        return mesh;
    }

    //void MeshComp::UpdateTransform(unsigned int entity, glm::mat4 transform) {
    //    unsigned int i = mIndexMap.at(entity);
    //    mModelMats[i] += transform;
    //}

}
