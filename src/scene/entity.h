#pragma once

#include <fstream>
#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace neith {
class Entity {
private:
    static std::unordered_map<std::string, unsigned int> mEntityNames;
    static std::vector<std::unordered_set<std::string>> mComponents;
    static std::vector<unsigned int> mParentIDs;
    static std::vector<std::vector<unsigned int>> mChildren;

public:
    static unsigned int AddEntity();
    static unsigned int AddEntity(std::string& name);
    static unsigned int AddEntity(unsigned int mParentID);

    static void SetParent(unsigned int entityID, unsigned int newParentID);

    static bool HasComponent(unsigned int entityID, std::string compName);
    static void AddComponent(unsigned int entityID, std::string compName);

    static void Transforme(unsigned int entityID, glm::mat4 transform);

    static int GetChildCount(unsigned int entityID);

    static unsigned int GetChild(unsigned int entityID, int index);

    static unsigned int GetParent(unsigned int entityID) { return mParentIDs.at(entityID); };

    static void Save(std::ofstream& file);
};
}  // namespace neith
