cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_FLAGS "-Wall")

add_library("${PROJECT_NAME}"
    "neith.h"
    "neith.cpp"
    "log.h"
    "log.cpp"
    "transform.h"
    "transform.cpp"
    "defaults.h"
    "render/framebuffer.h"
    "render/framebuffer.cpp"
    "render/render.h"
    "render/render.cpp"
	"render/shaders.h"
	"render/shaders.cpp"
    "render/instancerenderer.h"
    "render/instancerenderer.cpp"
    "render/textures.h"
    "render/textures.cpp"
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
    "scene/entity.h"
    "scene/entity.cpp"
    "scene/components/cameraComp.h"
    "scene/components/cameraComp.cpp"
    "scene/components/compManager.h"
    "scene/components/compManager.cpp"
    "scene/components/component.h"
    "scene/components/component.cpp"
    "scene/components/meshComp.h"
    "scene/components/meshComp.cpp"
    "scene/components/transformComp.h"
    "scene/components/transformComp.cpp"
    "scene/systems/sysCamera.h"
    "scene/systems/sysCamera.cpp"
    "scene/systems/sysMesh.h"
    "scene/systems/sysMesh.cpp"
    "scene/systems/sysTransform.h"
    "scene/systems/sysTransform.cpp"
    "scene/systems/sysMaterial.h"
    "scene/systems/sysMaterial.cpp"
    "window/input.h"
    "window/input.cpp"
    "window/window.h"
    "window/window.cpp"
    "dataStructures.h"
    "dataStructures.cpp"
	"timer.h"
	"timer.cpp"
)


target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/stb
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cgltf
    PUBLIC ${CMAKE_SOURCE_DIR}/external/spdlog/include
   	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries("${PROJECT_NAME}"
	glfw
	glad
    cgltf
    stb
    glm
    spdlog
)
