#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector>

namespace neith {
class Component {
protected:
    static std::unordered_map<unsigned int, unsigned int> mIndexMap;
};
}  // namespace neith
