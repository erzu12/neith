cmake_minimum_required(VERSION 3.16.2)

add_library("${PROJECT_NAME}"
    "neith.h"
    "neith.c"
    "physics/collision.h"
    "render/framebuffer.h"
    "render/framebuffer.c"
    "render/render.h"
    "render/render.c"
	"render/shaders.h"
    "render/staticrenderer.h"
    "render/textures.h"
	"scene/camera.h"
    "scene/material.h"
    "scene/mesh.h"
    "scene/meshloader.h"
    "scene/scene.h"
    "scene/skybox.h"
    "scene/skybox.c"
    "window/input.h"
    "window/input.c"
    "window/window.h"
    "window/window.c"
    "dataStructures.h"
	"stb_image.h"
	"time.h"
    "vecmath.h"
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
)
