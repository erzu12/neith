#pragma once

#include <vector>
#include <unordered_map>

#include <glm/mat4x4.hpp>

namespace neith {

    class Component {
public:
        static std::unordered_map<unsigned int, unsigned int> mIndexMap;
    };

}
