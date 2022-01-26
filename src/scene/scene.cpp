#include "scene.h"

#include "material.h"
#include "window/window.h"


//void DeleteStaticRender(struct RenderContext *rc) {
//    free(rc->VAOs);
//    free(rc);
//};


namespace neith {

    //unsigned int Scene::getComponentTypeID() {
        //static unsigned int lastID = 0;
        //components.push_back(new Component());
        //return lastID++;
    //}
    //template<typename T>
    //unsigned int Scene::getComponentTypeID() {
        //static unsigned int componentID = getComponentTypeID();
        //return componentID;
    //}

    Scene::Scene(int maxPrimitives) {
        sp = new StaticPrimitives(maxPrimitives);
        mat = new Materials(maxPrimitives);
        cd = new Camera();
    }

    Scene::~Scene() {
        delete mat;
        delete sp;
        delete cd; 
    }
}
