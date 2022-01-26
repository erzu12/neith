cmake_minimum_required(VERSION 3.16.2)

project(neith)

add_subdirectory(src)
add_subdirectory(testapp/src)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/cgltf)
add_subdirectory(external/stb)
add_subdirectory(external/glm)

set_target_properties(neith PROPERTIES CXX_STANDARD 11 LINKER_LANGUAGE CXX)
set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 11)

target_include_directories("${PROJECT_NAME}"
	PUBLIC include
)
