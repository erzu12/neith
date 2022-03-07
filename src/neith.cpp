#include "neith.h"

#include "window/window.h"
#include "scene/meshloader.h"
#include "scene/scene.h"
#include "scene/material.h"
#include "render/shaders.h"

#include "scene/systems/sysTransform.h"

namespace neith {
    struct Window* nth_CreateWindow() {
        return CreateWindow();
    }

    Scene* nth_InitScene(int maxPrimitives) {
        return new Scene(maxPrimitives);
    }

    void nth_AttachSceneToWindow(struct Scene *sc, struct Window *win) {
        AttachSceneToWindow(sc, win);
    }

    void nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount) {
        //LoadModels(sc, paths, modelCount);
    }

    //int LoadModel(std::string path) {
    //    return -1;
    //}

    int LoadModel(std::string path, int &outMeshCount) {
        return ModelLoader::LoadModel1(path, outMeshCount);
    }

    Renderer *nth_InitRender(struct Scene *sc, struct Window *win) {
        return new Renderer(sc, win);
    }

    void nth_UpdateWindow(struct Window *win) {
        UpdateWindow(win);
    }

    void nth_UpdateRender(Renderer *re, struct Scene *sc) {
        re->UpdateRender(sc);
    }

    unsigned int nth_LoadAndCompileShaders(const char* vertexPath, const char* fragmentPath) {
        return LoadAndCompileShaders(vertexPath, fragmentPath);
    }

    void nth_SetShader(struct Materials *mat, int material, int shader) {
        mat->SetShader(material, shader);
    }

    int nth_SetShaderByName(struct Materials *mat, char *materialName, int shader) {
        return mat->SetShaderByName(materialName, shader);
    }

    void nth_SetTexture(struct Materials *mat, int material, int texture, char *bindingName) {
        mat->SetTexture(material, texture, bindingName);
    }

    int nth_SetTextureByName(struct Materials *mat, char *materialName, int texture, char *bindingName) {
        return mat->SetTextureByName(materialName, texture, bindingName);
    }


    void nth_SetValueByNameF(struct Materials *mat, char *materialName, char *bindingName, float value) {
        mat->SetValueByNameF(materialName, bindingName, value);
    }

    void nth_SetValueByNameV3(struct Materials *mat, char *materialName, char *bindingName, float *value) {
        mat->SetValueByNameV3(materialName, bindingName, value);
    }

    void nth_SetValueByNameV3v(struct Materials *mat, char *materialName, char *bindingName, float x, float y, float z) {
        mat->SetValueByNameV3v(materialName, bindingName, x, y, z);
    }

    void nth_Transform(unsigned int entityID, glm::mat4 &transform) {
        system::Transform(entityID, transform);
    }
}
