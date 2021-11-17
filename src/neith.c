#include "neith.h"

#include "window/window.h"
#include "scene/meshloader.h"
#include "render/render.h"
#include "scene/scene.h"

struct Window* nth_CreateWindow() {
    return CreateWindow();
}

void nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount) {
    LoadModels(sc, paths, modelCount);
}

void nth_InitRender(struct Scene *sc, struct Window *win) {
    InitRender(sc, win);
}

void nth_UpdateWindow(struct Window *win) {
    UpdateWindow(win);
}

void nth_UpdateRender(struct Scene *sc) {
    UpdateRender(sc);
}




