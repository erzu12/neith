#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

namespace neith {
class CameraComp {
public:
    CameraComp();

    static void AttachCamera(unsigned int entityID);

    static unsigned int GetEntityID() { return mEntityID; }

    static void Transform(glm::mat4 &transform) { mTransform = transform; }

    static glm::mat4 GetViewMat() { return glm::inverse(mTransform); }

    static glm::vec3 GetCameraPos() { return glm::vec3(mTransform[3][0], mTransform[3][1], mTransform[3][2]); }

private:
    static unsigned int mEntityID;
    static glm::mat4 mTransform;
};
}  // namespace neith
