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

void Entity::Save(std::ofstream &file)
{
    // long componentStart = file.tellp();
    // file.seekp(componentStart + sizeof(int));
    // for (auto component : mComponents) {
    // int nameSize = component.size();
    // file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
    // file.write(component.data(), nameSize);
    //}
    // long componentEnd = file.tellp();
    // file.seekp(componentStart);
    // int componentSize = componentEnd - componentStart + sizeof(int);
    // file.write(reinterpret_cast<const char *>(&componentSize), sizeof(int));
    // file.seekp(componentEnd);

    // file.write(reinterpret_cast<const char *>(&mID), sizeof(mID));
    // file.write(reinterpret_cast<const char *>(&mParentID), sizeof(mParentID));

    // long entityNameStart = file.tellp();
    // file.seekp(entityNameStart + sizeof(int));
    // for (auto entityName : mEntityNames) {
    // int nameSize = entityName.first.size();
    // file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
    // file.write(entityName.first.data(), nameSize);
    // file.write(reinterpret_cast<const char *>(&entityName.second), sizeof(unsigned int));
    //}
    // long entityNameEnd = file.tellp();
    // file.seekp(entityNameStart);
    // int entityNameSize = entityNameEnd - entityNameStart + sizeof(int);
    // file.write(reinterpret_cast<const char *>(&entityNameSize), sizeof(int));
    // file.seekp(entityNameEnd);
}
}  // namespace neith
