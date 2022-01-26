#pragma once

#include <glm/mat4x4.hpp>

namespace neith {
    namespace system {
        int AddStaticPrimitive(unsigned int entity,
                               glm::mat4 modelMat,
                               float *vertices,
                               int vertCount,
                               int *indices,
                               int indCount,
                               int material);
    }
}
