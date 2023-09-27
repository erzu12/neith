cmake_minimum_required(VERSION 3.16.2)

add_library("${PROJECT_NAME}"
    "neith.h"
    "neith.cpp"
    "log.h"
    "log.cpp"
    "debug.h"
    "debug.cpp"
    "debugInfo.h"
    "debugInfo.cpp"
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
    "render/linerenderer.h"
    "render/linerenderer.cpp"
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
    "scene/model.h"
    "scene/model.cpp"
    "scene/components/cameraComp.h"
    "scene/components/cameraComp.cpp"
    "scene/components/component.h"
    "scene/components/component.cpp"
    "scene/components/meshComp.h"
    "scene/components/meshComp.cpp"
    "scene/components/newMeshComp.h"
    "scene/components/newMeshComp.cpp"
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
    "scene/systems/sysPhysics.h"
    "scene/systems/sysPhysics.cpp"
    "physics/physics.h"
    "physics/physics.cpp"
    "physics/colliders.h"
    "physics/colliders.cpp"
    "gui/gui.h"
    "gui/gui.cpp"
    "procedural/openSimplex.h"
    "procedural/openSimplex.cpp"
    "procedural/openSimplex2F.h"
    "procedural/openSimplex2F.cpp"
    "procedural/voronoi.h"
    "procedural/voronoi.cpp"
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
    PUBLIC ${CMAKE_SOURCE_DIR}/external/imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3Dynamics
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3Collision
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3Common
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3Geometry
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3OpenCL
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/Bullet3Serialize/Bullet2FileLoader
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/BulletDynamics
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/BulletCollision
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/BulletInverseDynamics
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/BulletSoftBody
    PUBLIC ${CMAKE_SOURCE_DIR}/external/bullet3/src/LinearMath
   	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries("${PROJECT_NAME}"
	glfw
	glad
    cgltf
    stb
    glm
    imgui
    Bullet3Dynamics
    Bullet3Collision
    Bullet3Common
    Bullet3Geometry
    Bullet3OpenCL_clew
    Bullet2FileLoader
    BulletDynamics
    BulletCollision
    BulletInverseDynamics
    BulletSoftBody
    LinearMath
    spdlog
)
