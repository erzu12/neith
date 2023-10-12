#include "rendercontext.h"

namespace neith {

PrimitiveRenderContext::PrimitiveRenderContext(std::vector<float> &vertices, std::vector<int> &indices, Material *material)
    : indCount(indices.size()), textureCount(material->getMaxTextureSlot()), material(material) {
    instanceCount = 0;

    unsigned int EBO;

    glGenBuffers(2, mVBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 12, vertices.data(),
            GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(),
            GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(sizeof(float) * 10));
    glEnableVertexAttribArray(3);

    // glGenBuffers(1, &VBOs[i][1]);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
    //NT_INTER_INFO("vbos: {}", mVBO[1]);
    // std::cout << "test1" << std::endl;
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    // glVertexAttribDivisor(4, 1);

    for (unsigned int j = 0; j < 4; j++) {
        glEnableVertexAttribArray(4 + j);
        glVertexAttribPointer(4 + j, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(float) * j * 4));
        glVertexAttribDivisor(4 + j, 1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PrimitiveRenderContext::setInstances(std::vector<glm::mat4> modelMatrices) {
    glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrices.size(), &modelMatrices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    instanceCount = modelMatrices.size();
}

}  // namespace neith
