#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

#include "scene/ecsmanager.h"
#include "scene/materialnew.h"

namespace neith {

struct PrimitiveRenderContext : Component {
    unsigned int index;
    size_t indCount;
    size_t textureCount;
    size_t instanceCount;
    MaterialNew *material;

    PrimitiveRenderContext(size_t indCount, size_t textureCount, MaterialNew *material)
        : indCount(indCount), textureCount(textureCount), material(material) {
        index = mPrimitiveCount++;
        instanceCount = 0;
    }
private:
    static unsigned int mPrimitiveCount;
};

class Primitive {
public:
    std::vector<float> vertices;
    std::vector<int> indices;
    MaterialNew *material;
};

class LOD {
    std::vector<Primitive> primitives;;
public:
    LOD();
    void AddPrimitive(Primitive primitive);
};

class Mesh {
    std::vector<LOD> LODs;
public:
    Mesh();
    void setLODs(std::vector<float> LODDistances);
    LOD *getLOD(int LODindex);
};

}  // namespace neith
