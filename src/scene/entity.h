#pragma once

namespace neith {
    struct Entity {

        unsigned int ID;
        unsigned int parentID;

        Entity(unsigned int ID);

        Entity(unsigned int ID, unsigned int parentID);
    
    };
}
