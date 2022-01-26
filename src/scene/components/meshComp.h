#pragma once

#include <unordered_map>
#include <vector>

#include <glm/mat4x4.hpp>

#include "component.h"

namespace neith {

    struct meshComp : public Component {
        int primitivesCount;
        int meshCount;
        glm::mat4 *modelMats;
        int vertSize;
        int indSize;
        float **vertices;
        int **indices;
        int *meshes;
        int *vertCounts;
        int *indCounts;
        int *materials;

        //StaticPrimitives(int maxPrimitives);

        int AddStaticPrimitive(glm::mat4 modelMat,
                               float *vertices,
                               int vertCount,
                               int *indices,
                               int indCount,
                               int material);

        int AddStaticMesh(int primitivesCount);

        //~StaticPrimitives();
    };
}
