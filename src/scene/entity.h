#pragma once

#include <vector>

#include <glm/mat4x4.hpp>

namespace neith {
    class Entity {
private:
        bool mHasComp[64] = { false };
        unsigned int mID;
        unsigned int mParentID;
        std::vector<Entity*> children;
public:
        Entity(unsigned int mID);

        void SetParent(unsigned int newParentID);

        void AddComponentByID(int compID);

        bool HasComponentByID(int compID);

        int GetID();

        void Transforme(glm::mat4 transform);

        int GetChildCount();

        Entity* GetChild(int index);

        Entity(unsigned int mID, unsigned int mParentID);
    
    };
}