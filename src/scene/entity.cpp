#include "entity.h"

#include <fstream>

#include "components/transformComp.h"

namespace neith {

std::unordered_map<std::string, unsigned int> Entity::mEntityNames;
std::vector<std::unordered_set<std::string>> Entity::mComponents;
std::vector<unsigned int> Entity::mParentIDs;
std::vector<std::vector<unsigned int>> Entity::mChildren;

unsigned int Entity::AddEntity() { return AddEntity(0); }
unsigned int Entity::AddEntity(std::string &name)
{
    unsigned int id = AddEntity();
    mEntityNames.insert({ name, id });
    return id;
}
unsigned int Entity::AddEntity(unsigned int parentID)
{
    unsigned int id = mParentIDs.size();

    mComponents.push_back(std::unordered_set<std::string>());
    mParentIDs.push_back(parentID);
    mChildren.push_back(std::vector<unsigned int>());

    TransformComp::AddTransform(id);
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
