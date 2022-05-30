#include "model.h"

namespace neith {
void Model::AddInstance(unsigned int instaceMeshes, glm::mat4 &instanceTransform)
{
    mInstaceMeshes.push_back(instaceMeshes);
    mInstanceTransform.push_back(instanceTransform);
}
}  // namespace neith
