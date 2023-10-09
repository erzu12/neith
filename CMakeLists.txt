cmake_minimum_required(VERSION 3.16.2)

project(neith)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)
add_subdirectory(testapp/src)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/cgltf)
add_subdirectory(external/stb)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/bullet3)
add_subdirectory(external/imgui)

#set(Bullet_DIR "external/bullet3/")

#if (DEFINED Bullet_DIR)
   #message("Finding local Bullet")
   #message(${Bullet_DIR})
   #Find_Package(Bullet CONFIG)
   ## SET(BULLET_INCLUDE_DIR ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_INCLUDE_DIR})
   #SET(BLA ${Bullet_DIR}/${BULLET_ROOT_DIR}/${BULLET_LIBRARY_DIRS})
#else()
   #message("Finding system installed Bullet")
   #Find_Package(Bullet)
#endif()


# show all cmake variables, for debugging
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#MESSAGE("BULLET_ROOT_DIR:")
#MESSAGE(${BULLET_ROOT_DIR})

#MESSAGE("Bullet_DIR:")
#MESSAGE(${Bullet_DIR})

#MESSAGE("BULLET_INCLUDE_DIR:")
#MESSAGE(${BULLET_INCLUDE_DIR})



set_target_properties(neith PROPERTIES CXX_STANDARD 23 LINKER_LANGUAGE CXX)
set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_include_directories("${PROJECT_NAME}"
	PUBLIC include
)
