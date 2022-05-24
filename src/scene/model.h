#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

namespace neith {
    class Model {
    public:
        void AddInstance(unsigned int instaceMeshes, glm::mat4 &instanceTransform);
        std::vector<unsigned int> *GetInstanceMeshes() {return &mInstaceMeshes; }
        std::vector<glm::mat4> *GetInstanceTransform() {return &mInstanceTransform; }
    private:
        std::vector<unsigned int> mInstaceMeshes;
        std::vector<glm::mat4> mInstanceTransform;
    };
}
