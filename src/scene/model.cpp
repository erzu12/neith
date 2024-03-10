#include "model.h"

namespace neith {
int Model::addMesh() {
    Mesh *mesh = new Mesh();
    mMeshes.push_back(ModelMesh(mesh, std::vector<glm::mat4x4>()));
    return mMeshes.size() - 1;
}

void Model::setInstances(std::vector<glm::mat4x4> modelMatrices) {
    for(auto &modelMesh : mMeshes) {
        std::vector<glm::mat4x4> newModelMatrices;
        newModelMatrices.reserve(modelMesh.modelMatrices.size() * modelMatrices.size());
        for(auto &modelMatrix : modelMatrices) {
            for(auto &mat : modelMesh.modelMatrices) {
                newModelMatrices.push_back(modelMatrix * mat);
            }
        }
        modelMesh.mesh->setInstances(newModelMatrices);
    }
}

std::vector<Material *> Model::getMaterials() {
    std::vector<Material *> materials;
    for(auto &modelMesh : mMeshes) {
        for(auto &lod : modelMesh.mesh->LODs) {
            for(auto &primitive : lod.primitives) {
                materials.push_back(primitive.material);
            }
        }
    }
    return materials;
}

void Model::addMeshModelMatrices(int meshIndex, std::vector<glm::mat4x4> modelMatrices) {
    mMeshes[meshIndex].modelMatrices = modelMatrices;
}

Model::~Model() {
    for(auto &modelMesh : mMeshes) {
        delete modelMesh.mesh;
    }
}
}  // namespace neith
