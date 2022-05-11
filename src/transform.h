#pragma once

#include <glm/mat4x4.hpp>

namespace neith {

class Transform {
public:
    Transform(glm::mat4 transform);
    glm::vec3 GetRight();
    glm::vec3 GetLeft();

private:
    glm::mat4 mTransform;
};
}  // namespace neith
