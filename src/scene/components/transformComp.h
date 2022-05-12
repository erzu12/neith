#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

#include "component.h"

namespace neith {

class TransformComp : public Component {
public:
    static void AddTransform(unsigned int entityID);
    static void AddTransform(unsigned int entityID, glm::mat4 mat);

    static void Transforme(unsigned int entity, glm::mat4 transform);
    static glm::mat4 GetTransform(unsigned int entityID);

private:
    static std::vector<glm::mat4> mTransform;
};

}  // namespace neith
