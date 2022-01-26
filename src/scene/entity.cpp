#include "entity.h"

namespace neith {

    Entity::Entity(unsigned int ID) {
        this->ID = ID;
    }

    Entity::Entity(unsigned int ID, unsigned int parentID) {
        this->ID = ID;
        this->parentID = parentID;
    }
}
