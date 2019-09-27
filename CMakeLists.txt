cmake_minimum_required(VERSION 3.10)

project(Ape)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include("BuildSystem/CompilerDefinitions.cmake")

include("BuildSystem/CompilerSettings.cmake")

SetStrictWarningPolicy()

SetCpp17StandardCompliance()

add_subdirectory(Ape)

add_subdirectory(Glad)

add_subdirectory(GLFWWindow)

add_subdirectory(GLFWTestApp)

add_subdirectory(QtTestApp)

add_subdirectory(QtWindow)

add_subdirectory(TestScene)

add_subdirectory(Stb)
