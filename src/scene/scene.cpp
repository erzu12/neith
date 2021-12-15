#include "scene.h"

#include <cglm/cglm.h>
#include "material.h"
#include "window/window.h"


//void DeleteStaticRender(struct RenderContext *rc) {
//    free(rc->VAOs);
//    free(rc);
//};

Scene::Scene(int maxPrimitives) {
    sp = new StaticPrimitives(maxPrimitives);
    mat = new Materials(maxPrimitives);
    cd = new Camera();
}

void Scene::InitRenderer(Scene *sc, struct Window *window) {
    re = new Renderer(sc, window);
}

void Scene::UpdateRender(Scene *sc) {
    re->UpdateRender(sc);
}

Scene::~Scene() {
    delete mat;
    //DeleteStaticRender(rc);
    delete sp;
    free(cd); 
}
