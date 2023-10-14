#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

#include "scene/ecsmanager.h"
#include "scene/material.h"

namespace neith {

class Primitive {
public:
    std::vector<float> vertices;
    std::vector<int> indices;
    Material *material;
    EntityHandle renderContextID;
};

class LOD {
public:
    std::vector<Primitive> primitives;
    LOD();
    void AddPrimitive(std::vector<float> vertices, std::vector<int> indices, Material *material);
    void AddPrimitive(Primitive primitive);
};

class Mesh {
    std::vector<LOD> LODs;
public:
    Mesh();
    void setLODs(std::vector<float> LODDistances);
    LOD *getLOD(int LODindex);
    void setInstances(std::vector<glm::mat4> modelMatrices);
};

}  // namespace neith
