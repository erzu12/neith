cmake_minimum_required(VERSION 3.16.2)

project(imgui)

add_library(${PROJECT_NAME}
    "imconfig.h"
    "imgui.cpp"
    "imgui.h"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_internal.h"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "examples/example_glfw_opengl3/main.cpp"
    "backends/imgui_impl_glfw.h"
    "backends/imgui_impl_glfw.cpp"
    "backends/imgui_impl_opengl3.h"
    "backends/imgui_impl_opengl3.cpp"
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC ${CMAKE_SOURCE_DIR}/external/imgui
    PUBLIC ${CMAKE_SOURCE_DIR}/external/imgui/backends
    PUBLIC ${CMAKE_SOURCE_DIR}/external/glfw/include
)
