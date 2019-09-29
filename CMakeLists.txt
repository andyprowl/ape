cmake_minimum_required(VERSION 3.10)

project(Ape)

set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include("BuildSystem/cmake/BuildSystem.cmake")

include("CMake/Dependencies.cmake")

ImportDependencies()

SetStrictWarningPolicy()

SetCpp17StandardCompliance()

add_subdirectory(BuildSystem)

add_subdirectory(Core)

add_subdirectory(Documentation)

add_subdirectory(Glad)

add_subdirectory(GLFWWindow)

add_subdirectory(GLFWTestApp)

add_subdirectory(QtTestApp)

add_subdirectory(QtWindow)

add_subdirectory(TestScene)

add_subdirectory(Stb)
