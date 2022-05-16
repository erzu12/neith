#include "transformComp.h"
#include "log.h"
#include <glm/gtx/string_cast.hpp>

namespace neith {
std::vector<glm::mat4> TransformComp::mTransform;
std::vector<glm::mat4> TransformComp::mGlobalTransform;

void TransformComp::AddTransform(unsigned int entityID, glm::mat4 &parentMat) { 
    glm::mat4 transform = glm::mat4(1.0f);
    AddTransform(entityID, parentMat, transform);
}

void TransformComp::AddTransform(unsigned int entityID, glm::mat4 &parentMat, glm::mat4 &mat) { 
    mTransform.push_back(mat);
    mGlobalTransform.push_back(mat * parentMat);
}

void TransformComp::Transforme(unsigned int entityID, glm::mat4 transform) { mTransform[entityID] *= transform; }

void TransformComp::TransformeGlobal(unsigned int entityID, glm::mat4 transform) { mGlobalTransform[entityID] *= transform; }

glm::mat4 TransformComp::GetTransform(unsigned int entityID) { return mTransform.at(entityID); }

glm::mat4 TransformComp::GetGlobalTransform(unsigned int entityID) { return mGlobalTransform.at(entityID); }
}  // namespace neith
