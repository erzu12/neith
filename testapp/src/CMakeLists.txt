cmake_minimum_required(VERSION 3.16.2)

project(testapp)

add_executable("${PROJECT_NAME}"
    "main.cpp"
    "camera.h"
    "camera.cpp"
)

set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries("${PROJECT_NAME}"
    neith
)
