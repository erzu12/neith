#include "scene.h"

#include <cglm/cglm.h>
#include "mesh.h"
#include "material.h"
#include "window/window.h"


void DeleteStaticRender(struct RenderContext *rc) {
    free(rc->VAOs);
    free(rc);
};

struct Scene *InitScene(int maxPrimitives) {
    struct Scene *scene = (struct Scene*)malloc(sizeof(struct Scene));
    scene->sp = InitStaticPrimitives(maxPrimitives);
    scene->mat = InitMaterials(maxPrimitives);
    scene->cd = new Camera();
    return scene;
}

void DeleteScene(struct Scene *sc) {
    DeleteMaterials(sc->mat);
    DeleteStaticRender(sc->rc);
    DeleteStaticPrimitives(sc->sp);
    free(sc->cd); 
}
