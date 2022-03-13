#include "sysMaterial.h"

#include "scene/material.h"
#include "scene/components/meshComp.h"

namespace neith {
    namespace system {
        void SetShader(unsigned int meshID, int material, int shader) {
            Materials::SetShader(MeshComp::getMaterial(meshID, material), shader);
        }

        void SetTexture(unsigned int meshID, int material, int texture, const char *bindingName) {
            Materials::SetTexture(MeshComp::getMaterial(meshID, material), texture, bindingName);
        }

        void SetValue(unsigned int meshID, int material, const char *bindingName, float value) {
            Materials::SetValue(MeshComp::getMaterial(meshID, material), bindingName, value);
        }

        void SetValue(unsigned int meshID, int material, const char *bindingName, float *value) {
            Materials::SetValue(MeshComp::getMaterial(meshID, material), bindingName, value);
        }

        void SetValue(unsigned int meshID, int material, const char *bindingName, float x, float y, float z) {
            Materials::SetValue(MeshComp::getMaterial(meshID, material), bindingName, x, y, z);
        }
    }
}
