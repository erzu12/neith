#pragma once

#include <vector>
#include "scene/ecsmanager.h"
#include "scene/material.h"

namespace neith {

struct PrimitiveRenderContext : Component {
    size_t indCount;
    size_t textureCount;
    size_t instanceCount;
    Material *material;

    unsigned int mVAO;
    unsigned int mVBO[2];

    PrimitiveRenderContext(std::vector<float> &vertices, std::vector<int> &indices, Material *material);
    void setInstances(std::vector<glm::mat4> modelMatrices);

    private:
    static unsigned int mPrimitiveCount;
};

}  // namespace neith
