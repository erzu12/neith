cmake_minimum_required(VERSION 3.16.2)

project(neith)

add_subdirectory(src)
add_subdirectory(testapp/src)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/cglm)
add_subdirectory(external/cgltf)
add_subdirectory(external/stb)

set_target_properties(neith PROPERTIES C_STANDARD 11 LINKER_LANGUAGE C)

target_include_directories("${PROJECT_NAME}"
	PUBLIC include
)
