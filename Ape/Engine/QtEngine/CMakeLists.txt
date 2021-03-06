cmake_minimum_required(VERSION 3.10)

project(QtEngine)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(
    ${PROJECT_NAME}
    Ape::Engine::Windowing
    Ape::Rendering::Rendering
    3rdParty::Glad
    3rdParty::Qt5::Core
    3rdParty::Qt5::OpenGL
    3rdParty::Qt5::Widgets)

AddPrivateDependencies(
    ${PROJECT_NAME}
    opengl32
    3rdParty::DearImGui
    Ape::Engine::FrameProfiling
    Ape::Engine::Initialization
    Ape::Engine::InspectionOverlay
    Ape::Engine::UpdateHandling
    Basix::Container
    Basix::Time)

file(GLOB MOC_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/mocs_compilation.cpp")

source_group("Generated Files" FILES ${MOC_FILES})
