#include "transformComp.h"

#include <glm/gtx/string_cast.hpp>

#include "log.h"

namespace neith {
//std::vector<glm::mat4> TransformComp::mTransform;
//std::vector<glm::mat4> TransformComp::mGlobalTransform;

//void TransformComp::AddTransform(unsigned int entityID, glm::mat4 &parentMat)
//{
    //glm::mat4 transform = glm::mat4(1.0f);
    //AddTransform(entityID, parentMat, transform);
//}

//void TransformComp::AddTransform(unsigned int entityID, glm::mat4 &parentMat, glm::mat4 &mat)
//{
    //mTransform.push_back(mat);
    //mGlobalTransform.push_back(mat * parentMat);
//}

//void TransformComp::Transforme(unsigned int entityID, glm::mat4 transform) { mTransform[entityID] *= transform; }

//void TransformComp::TransformeGlobal(unsigned int entityID, glm::mat4 transform)
//{
    //mGlobalTransform[entityID] *= transform;
//}

//glm::mat4 TransformComp::GetTransform(unsigned int entityID) { return mTransform.at(entityID); }

//glm::mat4 TransformComp::GetGlobalTransform(unsigned int entityID) { return mGlobalTransform.at(entityID); }

//DebugInfo *TransformComp::GetDebugInfo(unsigned int entityID)
//{
    //DebugInfo *nameInfo = new DebugInfo("Transform Component");
    //DebugInfo *debugInfo = new DebugInfo("Transforms:");
    //debugInfo->child.push_back(new DebugInfo("Transform:\n" + glm::to_string(mTransform.at(entityID))));
    //debugInfo->child.push_back(new DebugInfo("Global Transform:\n" + glm::to_string(mGlobalTransform.at(entityID))));
    //nameInfo->child.push_back(debugInfo);
    //return nameInfo;
//}
}  // namespace neith
