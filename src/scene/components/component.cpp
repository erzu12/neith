#include "component.h"

namespace neith {
std::unordered_map<unsigned int, unsigned int> Component::mIndexMap;
DebugInfo* Component::GetDebugInfo(unsigned int entityID)
{
    DebugInfo* debugInfo = new DebugInfo("Component");
    debugInfo->child.push_back(new DebugInfo("no details"));
    return debugInfo;
}

void Component::Transform(unsigned int entityID, glm::mat4& transform) {}

}  // namespace neith
