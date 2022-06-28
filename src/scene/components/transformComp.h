#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

#include "component.h"

namespace neith {

class TransformComp : public Component {
public:
    static TransformComp *GetInstance()
    {
        static TransformComp instance;
        return &instance;
    }
    static void AddTransform(unsigned int entityID, glm::mat4 &parentMat);
    static void AddTransform(unsigned int entityID, glm::mat4 &parentMat, glm::mat4 &mat);

    static void Transforme(unsigned int entity, glm::mat4 transform);
    static void TransformeGlobal(unsigned int entity, glm::mat4 transform);
    static glm::mat4 GetTransform(unsigned int entityID);
    static glm::mat4 GetGlobalTransform(unsigned int entityID);

    DebugInfo *GetDebugInfo(unsigned int entityID) override;

private:
    TransformComp(){};
    static std::vector<glm::mat4> mTransform;
    static std::vector<glm::mat4> mGlobalTransform;

public:
    TransformComp(TransformComp &other) = delete;
    void operator=(const TransformComp &) = delete;
};

}  // namespace neith
