cmake_minimum_required(VERSION 3.10)

project(Ape)

set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})

include("BuildSystem/cmake/BuildSystem.cmake")

include("CMake/Dependencies.cmake")

set(CMAKE_PREDEFINED_TARGETS_FOLDER "Infrastructure")

EnableIDEFolders()

ImportDependencies()

SetStrictWarningPolicy()

SetCpp17StandardCompliance()

add_subdirectory(AssetLoader)

add_subdirectory(BuildSystem)

add_subdirectory(Core)

add_subdirectory(Documentation)

add_subdirectory(Glad)

add_subdirectory(GlfwBinding)

add_subdirectory(GLFWTestApp)

add_subdirectory(QtBinding)

add_subdirectory(QtTestApp)

add_subdirectory(TestScene)

add_subdirectory(Stb)
