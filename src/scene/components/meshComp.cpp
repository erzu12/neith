#include "meshComp.h"


namespace neith {

    int MeshComp::mPrimitivesCount = 0;
    int MeshComp::mMeshCount = 0;
    std::vector<std::vector<glm::mat4>> MeshComp::mModelMats;
    std::vector<int> MeshComp::mInstanceCount;
    std::vector<std::unordered_map<unsigned int, unsigned int>> MeshComp::mInstanceMap;
    std::vector<std::vector<bool>> MeshComp::mUpdate;
    std::vector<float*> MeshComp::mVertices;
    std::vector<int*> MeshComp::mIndices;
    std::vector<int> MeshComp::mMeshes;
    std::vector<int> MeshComp::mVertCounts;
    std::vector<int> MeshComp::mIndCounts;
    std::vector<int> MeshComp::mMaterials;

    int MeshComp::AddStaticPrimitive(float *vertices,
                                             int vertCount,
                                             int *indices,
                                             int indCount,
                                             int material)
    {
        mVertices.push_back(vertices);
        mIndices.push_back(indices);
        mVertCounts.push_back(vertCount);
        mIndCounts.push_back(indCount);
        mMaterials.push_back(material);
        mUpdate.push_back(std::vector<bool>{false});

        mPrimitivesCount = mModelMats.size();

        return mPrimitivesCount;
    }

    int MeshComp::AddStaticMesh(int primitivesCount) {
        int mesh = mMeshCount;
        mMeshCount++;

        int primitive = primitivesCount;
        primitivesCount = primitive + primitivesCount;

        mMeshes.push_back(primitivesCount);
        mMeshes.push_back(0);
        mMeshes.push_back(primitivesCount + primitivesCount);

        return mesh;
    }

    //void MeshComp::UpdateTransform(unsigned int entity, glm::mat4 transform) {
    //    unsigned int i = mIndexMap.at(entity);
    //    mModelMats[i] += transform;
    //}

}
