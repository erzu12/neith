#include "entity.h"

#include <algorithm>
#include <fstream>
#include <string>

#include "components/transformComp.h"
#include "log.h"

namespace neith {

std::unordered_map<std::string, unsigned int> Entity::mEntityNameMap;
std::vector<std::string> Entity::mEntityNames;
std::vector<std::unordered_set<Component *>> Entity::mComponents;
std::vector<unsigned int> Entity::mParentIDs;
std::vector<std::vector<unsigned int>> Entity::mChildren;

void Entity::InitEntites()
{
    mComponents.push_back(std::unordered_set<Component *>());
    mParentIDs.push_back(0);
    mChildren.push_back(std::vector<unsigned int>());
    mEntityNames.push_back("Scene");
    mEntityNameMap.insert({ "Scene", 0 });

    glm::mat4 parentMat = glm::mat4(1.0f);
    TransformComp::AddTransform(0, parentMat);
}

unsigned int Entity::AddEntity(unsigned int parentID, glm::mat4 &transform, std::string &name)
{
    unsigned int id = mParentIDs.size();

    mComponents.push_back(std::unordered_set<Component *>());
    mParentIDs.push_back(parentID);
    mChildren.at(parentID).push_back(id);
    mChildren.push_back(std::vector<unsigned int>());

    glm::mat4 parentMat = TransformComp::GetGlobalTransform(parentID);
    TransformComp::AddTransform(id, parentMat, transform);
    AddComponent(id, TransformComp::GetInstance());

    bool result = mEntityNameMap.insert({ name, id }).second;
    if (result) {
        mEntityNames.push_back(name);
    }
    int count = 0;
    while (!result) {
        result = mEntityNameMap.insert({ name + "." + std::to_string(count), id }).second;
        if (result) {
            mEntityNames.push_back(name + "." + std::to_string(count));
        }
        count++;
    }
    return id;
}

unsigned int Entity::AddEntity(unsigned int parentID, glm::mat4 &transform)
{
    return AddEntity(parentID, transform, mEntityNames.at(parentID));
}

void Entity::SetParent(unsigned int entityID, unsigned int newParentID)
{
    unsigned int oldParent = mParentIDs.at(entityID);

    std::vector<unsigned int>::iterator position =
        std::find(mChildren.at(oldParent).begin(), mChildren.at(oldParent).end(), entityID);
    if (position != mChildren.at(oldParent).end()) {
        mChildren.at(oldParent).erase(position);
    }

    mParentIDs.at(entityID) = newParentID;
    mChildren.at(newParentID).push_back(entityID);
}

void Entity::AddComponent(unsigned int entityID, Component *comp) { mComponents.at(entityID).insert(comp); }

// bool Entity::HasComponent(unsigned int entityID, std::string compName)
//{
//     return mComponents.at(entityID).count(compName);
// }

void Entity::Transforme(unsigned int entityID, glm::mat4 transform) { TransformComp::Transforme(entityID, transform); }

int Entity::GetChildCount(unsigned int entityID) { return mChildren.at(entityID).size(); }

unsigned int Entity::GetChild(unsigned int entityID, int index) { return mChildren.at(entityID).at(index); }

}  // namespace neith
