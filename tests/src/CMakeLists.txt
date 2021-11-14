cmake_minimum_required(VERSION 3.16.2)

add_executable("${PROJECT_NAME}"
    "maintest.c"
    "collisionTest.h"
)

target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/tests/cu
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/stb
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cglm/includes
	PUBLIC ${CMAKE_SOURCE_DIR}/external/cgltf
)

target_link_libraries("${PROJECT_NAME}"
	cu
	glfw
	glad
	cglm
    cgltf
)
