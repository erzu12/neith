#pragma once

#include <glm/mat4x4.hpp>

namespace neith {
    namespace system {
        int AddStaticPrimitive(float *vertices,
                               int vertCount,
                               int *indices,
                               int indCount,
                               int material);

    int AddMesh(int primitivesCount);

    void AddMeshToEntity(unsigned int entityID, unsigned int meshID, glm::mat4 modelMat);
    }
}
