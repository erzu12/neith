#pragma once

#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector>

#include "debugInfo.h"

namespace neith {
class Component {
public:
    virtual DebugInfo* GetDebugInfo(unsigned int entityID);
    virtual void Transform(unsigned int entityID, glm::mat4& transform);

protected:
    static std::unordered_map<unsigned int, unsigned int> mIndexMap;
};
}  // namespace neith
