#include "transformComp.h"

namespace neith {
std::vector<glm::mat4> TransformComp::mTransform;

void TransformComp::AddTransform(unsigned int entityID) { AddTransform(entityID, glm::mat4(1.0f)); }

void TransformComp::AddTransform(unsigned int entityID, glm::mat4 mat) { mTransform.push_back(mat); }

void TransformComp::Transforme(unsigned int entityID, glm::mat4 transform) { mTransform[entityID] += transform; }

glm::mat4 TransformComp::GetTransform(unsigned int entityID) { return mTransform.at(entityID); }
}  // namespace neith
