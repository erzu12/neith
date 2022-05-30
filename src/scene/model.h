#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

namespace neith {
class Model {
public:
    void AddInstance(unsigned int instaceMeshes, glm::mat4 &instanceTransform);
    std::vector<unsigned int> *GetInstanceMeshes() { return &mInstaceMeshes; }
    std::vector<glm::mat4> *GetInstanceTransform() { return &mInstanceTransform; }
    std::vector<unsigned int> *GetMaterials() { return &mMaterials; }

private:
    std::vector<unsigned int> mInstaceMeshes;
    std::vector<glm::mat4> mInstanceTransform;
    std::vector<unsigned int> mMaterials;
};
}  // namespace neith
