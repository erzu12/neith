#include "model.h"

namespace neith {
Mesh *Model::addMesh() {
    Mesh *mesh = new Mesh();
    mMeshes.push_back(mesh);
    return mesh;
}

void Model::setInstances(std::vector<glm::mat4x4> modelMatrices) {
    for(auto &mesh : mMeshes) {
        mesh->setInstances(modelMatrices);
    }
}

Model::~Model() {
    for(auto &mesh : mMeshes) {
        delete mesh;
    }
}
}  // namespace neith
