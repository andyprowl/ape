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

add_subdirectory(ApePlayer)

add_subdirectory(ApeCreator)

add_subdirectory(Asset)

add_subdirectory(AssetLoader)

add_subdirectory(BuildSystem)

add_subdirectory(CompilerSupport)

add_subdirectory(Documentation)

add_subdirectory(Engine)

add_subdirectory(Glad)

add_subdirectory(GlfwEngine)

add_subdirectory(Glow)

add_subdirectory(Mathematics)

add_subdirectory(QtEngine)

add_subdirectory(Range)

add_subdirectory(Scene)

add_subdirectory(Signal)

add_subdirectory(Stb)

add_subdirectory(TestScene)
