/*
 * Include this file in your Project.
 */

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene/scene.h"
#include "window/window.h"

struct Window* nth_CreateWindow();

void nth_LoadModels(struct Scene *sc, const char* paths[], int modelCount);

void nth_InitRender(struct Scene *sc, struct Window *win);

void nth_UpdateWindow(struct Window *win);

void nth_UpdateRender(struct Scene *sc);
