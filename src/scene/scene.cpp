#include "scene.h"

#include <fstream>
#include <iostream>
#include <string>

#include "log.h"
#include "material.h"
//#include "scene/components/transformComp.h"
#include "window/window.h"

// void DeleteStaticRender(struct RenderContext *rc) {
//     free(rc->VAOs);
//     free(rc);
// };

namespace neith {

// unsigned int Scene::getComponentTypeID() {
// static unsigned int lastID = 0;
// components.push_back(new Component());
// return lastID++;
//}
// template<typename T>
// unsigned int Scene::getComponentTypeID() {
// static unsigned int componentID = getComponentTypeID();
// return componentID;
//}

Scene::Scene()
{
    Materials::AddMaterial();
}

glm::vec3 Scene::GetLightDir() { return lightDir; }

void Scene::Save(std::ofstream file) {}

Scene::~Scene()
{
    // delete sp;
}
}  // namespace neith
