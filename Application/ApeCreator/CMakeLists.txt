cmake_minimum_required(VERSION 3.10)

project(ApeCreator)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    Ape::QtEngine
    Application::TestScene)

file(GLOB MOC_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp")

source_group("Generated Files" FILES ${MOC_FILES})
