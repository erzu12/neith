#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include "scene/mesh.h"

namespace neith {
class Model {
public:
    ~Model();
    void setInstances(std::vector<glm::mat4x4> modelMatrices);
    Mesh *addMesh();
    std::vector<Mesh*> *getMeshes() { return &mMeshes; }
    //std::vector<unsigned int> *GetMaterials() { return &mMaterials; }

private:
    //std::vector<unsigned int> mMaterials;
    std::vector<Mesh*> mMeshes;
};
}  // namespace neith
