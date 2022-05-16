#include "entity.h"

#include <fstream>

#include "components/transformComp.h"
#include "log.h"

namespace neith {

std::unordered_map<std::string, unsigned int> Entity::mEntityNames;
std::vector<std::unordered_set<std::string>> Entity::mComponents;
std::vector<unsigned int> Entity::mParentIDs;
std::vector<std::vector<unsigned int>> Entity::mChildren;

void Entity::InitEntites() {
    mComponents.push_back(std::unordered_set<std::string>());
    mParentIDs.push_back(0);
    mChildren.push_back(std::vector<unsigned int>());
    mEntityNames.insert({ "Scene", 0 });

    glm::mat4 parentMat = glm::mat4(1.0f);
    TransformComp::AddTransform(0, parentMat);
}

unsigned int Entity::AddEntity(unsigned int parentID, glm::mat4 &transform, std::string &name)
{
    unsigned int id = AddEntity(parentID, transform);
    mEntityNames.insert({ name, id });
    return id;
}

unsigned int Entity::AddEntity(unsigned int parentID, glm::mat4 &transform)
{
    unsigned int id = mParentIDs.size();

    mComponents.push_back(std::unordered_set<std::string>());
    mParentIDs.push_back(parentID);
    mChildren.push_back(std::vector<unsigned int>());

    glm::mat4 parentMat = TransformComp::GetGlobalTransform(parentID);
    TransformComp::AddTransform(id, parentMat, transform);
    return id;
}

void Entity::SetParent(unsigned int entityID, unsigned int newParentID) { mParentIDs.at(entityID) = newParentID; }

void Entity::AddComponent(unsigned int entityID, std::string compName) { mComponents.at(entityID).insert(compName); }

bool Entity::HasComponent(unsigned int entityID, std::string compName)
{
    return mComponents.at(entityID).count(compName);
}

void Entity::Transforme(unsigned int entityID, glm::mat4 transform) { TransformComp::Transforme(entityID, transform); }

int Entity::GetChildCount(unsigned int entityID) { return mChildren.at(entityID).size(); }

unsigned int Entity::GetChild(unsigned int entityID, int index) { return mChildren.at(entityID).at(index); }

}  // namespace neith
