#include "mesh.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "material.h"

namespace neith {

LOD::LOD() {
    primitives = std::vector<Primitive>();
}    

void LOD::AddPrimitive(Primitive primitive) {
    primitives.push_back(primitive);
    ECSManager::ecs.createEntity(
        PrimitiveRenderContext(
            primitive.indices.size(), 
            primitive.material->getMaxTextureSlot(),
            primitive.material
        )
    );
}

Mesh::Mesh() {
    LODs = std::vector<LOD>();
    LODs.push_back(LOD());
}

void Mesh::setLODs(std::vector<float> LODDistances) {
    for (auto LODDistance : LODDistances) {
        LODs.push_back(LOD());
    }
}

LOD *Mesh::getLOD(int LODindex) {
    return &LODs.at(LODindex);
}

}  // namespace neith
