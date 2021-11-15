cmake_minimum_required(VERSION 3.16.2)

project(neithTest)


add_subdirectory(src)
add_subdirectory(cu)

set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD 11)

target_include_directories("${PROJECT_NAME}"
	PUBLIC include
)
