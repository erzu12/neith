cmake_minimum_required(VERSION 3.16.2)


add_library("${PROJECT_NAME}"
    "neith.h"
    "neith.cpp"
    "defaults.h"
    "physics/collision.h"
    "physics/collision.cpp"
    "render/framebuffer.h"
    "render/framebuffer.cpp"
    "render/render.h"
    "render/render.cpp"
	"render/shaders.h"
	"render/shaders.cpp"
    "render/staticrenderer.h"
    "render/staticrenderer.cpp"
    "render/textures.h"
    "render/textures.cpp"
	"scene/camera.h"
	"scene/camera.cpp"
    "scene/material.h"
    "scene/material.cpp"
    "scene/mesh.h"
    "scene/mesh.cpp"
    "scene/meshloader.h"
    "scene/meshloader.cpp"
    "scene/scene.h"
    "scene/scene.cpp"
    "scene/skybox.h"
    "scene/skybox.cpp"
    "window/input.h"
    "window/input.cpp"
    "window/window.h"
    "window/window.cpp"
    "dataStructures.h"
    "dataStructures.cpp"
	"time.h"
	"time.cpp"
    "vecmath.h"
    "vecmath.cpp"
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
