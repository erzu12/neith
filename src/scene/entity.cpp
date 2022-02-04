#include "entity.h"

#include "components/transformComp.h"

namespace neith {

    Entity::Entity(unsigned int ID) {
        mID = ID;
        TransformComp::AddTransform(ID);

    }

    void Entity::SetParent(unsigned int newParentID) {
        mParentID = newParentID;
    }

    void Entity::AddComponentByID(int compID) {
        mHasComp[compID] = true;
    }

    bool Entity::HasComponentByID(int compID) {
        return mHasComp[compID];
    }

    int Entity::GetID() {
        return mID;
    }

    void Entity::Transforme(glm::mat4 transform) {
        TransformComp::Transforme(mID, transform);
    }

    int Entity::GetChildCount() {
        return children.size();
    }

    Entity* Entity::GetChild(int index) {
        return children.at(index);
    }

    Entity::Entity(unsigned int ID, unsigned int parentID) {
        mID = ID;
        mParentID = parentID;
        TransformComp::AddTransform(ID);
    }
}
