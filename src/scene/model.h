#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>
#include "scene/mesh.h"

namespace neith {

struct ModelMesh {
    Mesh *mesh;
    std::vector<glm::mat4x4> modelMatrices;
};

class Model {
public:
    ~Model();
    void setInstances(std::vector<glm::mat4x4> modelMatrices);
    int addMesh();
    void addMeshModelMatrices(int meshIndex, std::vector<glm::mat4x4> modelMatrices);
    std::vector<ModelMesh> *getMeshes() { return &mMeshes; }
    std::vector<Material *> getMaterials();

private:
    std::vector<ModelMesh> mMeshes;
};
}  // namespace neith
