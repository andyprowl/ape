cmake_minimum_required(VERSION 3.10)

project(Ape)

set(SOURCE_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_PREDEFINED_TARGETS_FOLDER "Infrastructure")

include("Infrastructure/BuildSystem/cmake/BuildSystem.cmake")

InitializeBuildSystem()

SetStrictWarningPolicy()

SetCpp17StandardCompliance()

add_subdirectory(3rdParty)

add_subdirectory(Ape)

add_subdirectory(Application)

add_subdirectory(Foundational)

add_subdirectory(Infrastructure)
