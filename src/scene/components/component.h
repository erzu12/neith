#pragma once

#include <vector>
#include <unordered_map>

#include <glm/mat4x4.hpp>

namespace neith {

    struct Component {
        static std::unordered_map<int, int> indexMap;
        static std::vector<glm::mat4> transform;
    };

}
