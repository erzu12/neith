cmake_minimum_required(VERSION 3.16.2)

project(testapp)

add_executable("${PROJECT_NAME}"
    "main.c"
)

set_property(TARGET "${PROJECT_NAME}" PROPERTY C_STANDARD 11)

target_include_directories("${PROJECT_NAME}"
	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries("${PROJECT_NAME}"
    neith
)
