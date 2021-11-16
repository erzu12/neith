cmake_minimum_required(VERSION 3.16.2)

add_library("${PROJECT_NAME}"
    "neith.h"
    "vecmath.h"
    "scene.h"
    "collision.h"
    "staticrenderer.h"
	"camera.h"
	"shaders.h"
    "textures.h"
    "mesh.h"
    "meshloader.h"
	"time.h"
    "material.h"
    "dataStructures.h"
	"stb_image.h"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/stb
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cglm/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cgltf
)

target_link_libraries("${PROJECT_NAME}"
	glfw
	glad
	cglm
    cgltf
)
