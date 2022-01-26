#include "transformComp.h"

namespace neith {
    std::vector<glm::mat4> TransformComp::mTransform;

    void TransformComp::AddTransform(unsigned int entity) {
        AddTransform(entity, glm::mat4(1.0f));
    }

    void TransformComp::AddTransform(unsigned int entity, glm::mat4 mat) {
        mTransform.push_back(mat);
    }

    void TransformComp::Transforme(unsigned int entity, glm::mat4 transform) {
        mTransform[entity] += transform;
    }
}
