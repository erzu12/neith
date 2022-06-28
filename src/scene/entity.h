#pragma once

#include <fstream>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "components/component.h"

namespace neith {
class Entity {
private:
    static std::unordered_map<std::string, unsigned int> mEntityNameMap;
    static std::vector<std::string> mEntityNames;
    static std::vector<std::unordered_set<Component*>> mComponents;
    static std::vector<unsigned int> mParentIDs;
    static std::vector<std::vector<unsigned int>> mChildren;

public:
    static void InitEntites();
    static unsigned int AddEntity(unsigned int mParentID, glm::mat4& transform);
    static unsigned int AddEntity(unsigned int mParentID, glm::mat4& transform, std::string& name);

    static void SetParent(unsigned int entityID, unsigned int newParentID);

    // static bool HasComponent(unsigned int entityID, std::string compName);
    static std::unordered_set<Component*> GetComponents(unsigned int entityID) { return mComponents.at(entityID); }
    static void AddComponent(unsigned int entityID, Component* component);

    static void Transforme(unsigned int entityID, glm::mat4 transform);

    static std::string GetName(unsigned int entityID) { return mEntityNames.at(entityID); }

    static int GetChildCount(unsigned int entityID);

    static unsigned int GetChild(unsigned int entityID, int index);

    static unsigned int GetParent(unsigned int entityID) { return mParentIDs.at(entityID); };

    static void Save(std::ofstream& file);
};
}  // namespace neith
