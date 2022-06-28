#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

namespace neith {
class Model {
public:
    void AddInstance(unsigned int instaceMeshes, glm::mat4 &instanceTransform, std::string name);
    std::vector<unsigned int> *GetInstanceMeshes() { return &mInstanceMeshes; }
    std::vector<glm::mat4> *GetInstanceTransform() { return &mInstanceTransform; }
    std::vector<std::string> *GetInstanceNames() { return &mInstanceNames; }
    std::vector<unsigned int> *GetMaterials() { return &mMaterials; }

private:
    std::vector<unsigned int> mInstanceMeshes;
    std::vector<glm::mat4> mInstanceTransform;
    std::vector<std::string> mInstanceNames;
    std::vector<unsigned int> mMaterials;
};
}  // namespace neith
