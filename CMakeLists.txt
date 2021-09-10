cmake_minimum_required(VERSION 3.16.2)

project(neith)


add_subdirectory(src)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/cglm)

set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD 11)

target_include_directories("${PROJECT_NAME}"
	PUBLIC include
)
