cmake_minimum_required(VERSION 3.16.2)

add_library("${PROJECT_NAME}"
    "neith.h"
    "neith.c"
    "defaults.h"
    "physics/collision.h"
    "physics/collision.c"
    "render/framebuffer.h"
    "render/framebuffer.c"
    "render/render.h"
    "render/render.c"
	"render/shaders.h"
	"render/shaders.c"
    "render/staticrenderer.h"
    "render/staticrenderer.c"
    "render/textures.h"
    "render/textures.c"
	"scene/camera.h"
	"scene/camera.c"
    "scene/material.h"
    "scene/material.c"
    "scene/mesh.h"
    "scene/mesh.c"
    "scene/meshloader.h"
    "scene/meshloader.c"
    "scene/scene.h"
    "scene/scene.c"
    "scene/skybox.h"
    "scene/skybox.c"
    "window/input.h"
    "window/input.c"
    "window/window.h"
    "window/window.c"
    "dataStructures.h"
    "dataStructures.c"
	"time.h"
	"time.c"
    "vecmath.h"
    "vecmath.c"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/stb
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cglm/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cgltf
	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries("${PROJECT_NAME}"
	glfw
	glad
	cglm
    cgltf
    stb
)
