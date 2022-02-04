#pragma once

#include <vector>

#include "component.h"

#include <glm/mat4x4.hpp>

namespace neith {

    class TransformComp : public Component {
public:
        static std::vector<glm::mat4> mTransform;
        static void AddTransform(unsigned int entity);
        static void AddTransform(unsigned int entity, glm::mat4 mat);
        
        static void Transforme(unsigned int entity, glm::mat4 transform);
    };

}
