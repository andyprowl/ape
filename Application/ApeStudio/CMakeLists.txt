cmake_minimum_required(VERSION 3.10)

project(ApeStudio)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

AddExecutable(${PROJECT_NAME})

AddPrivateDependencies(
    ${PROJECT_NAME}
    Ape::Initialization
    Ape::QtEngine
    Application::RaveCore)

file(GLOB MOC_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp")

source_group("Generated Files" FILES ${MOC_FILES})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VS_DEBUGGER_COMMAND_ARGUMENTS
    "--enable-debug-output")
