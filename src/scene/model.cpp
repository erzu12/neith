#include "model.h"

namespace neith {
void Model::AddInstance(unsigned int instaceMeshes, glm::mat4 &instanceTransform, std::string instanceName)
{
    mInstanceMeshes.push_back(instaceMeshes);
    mInstanceTransform.push_back(instanceTransform);
    mInstanceNames.push_back(instanceName);
}
}  // namespace neith
