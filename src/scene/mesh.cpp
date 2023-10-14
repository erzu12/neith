#include "mesh.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "material.h"
#include "rendercontext.h"

namespace neith {


LOD::LOD() {
    primitives = std::vector<Primitive>();
}    

void LOD::AddPrimitive(Primitive primitive) {
    primitives.push_back(primitive);
}

void LOD::AddPrimitive(std::vector<float> vertices, std::vector<int> indices, Material *material) {
    Primitive primitive;
    primitive.vertices = vertices;
    primitive.indices = indices;
    primitive.material = material;
    primitive.renderContextID = ECSManager::ecs.createEntity(
        PrimitiveRenderContext(
            primitive.vertices,
            primitive.indices,
            primitive.material
        )
    );
    primitives.push_back(primitive);
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

void Mesh::setInstances(std::vector<glm::mat4> modelMatrices) {
    for (auto LOD : LODs) {
        for (auto primitive : LOD.primitives) {
            ECSManager::ecs.get<PrimitiveRenderContext>(primitive.renderContextID, [&](PrimitiveRenderContext *renderContext) {
                renderContext->setInstances(modelMatrices);
            });
        }
    }
}

}  // namespace neith
