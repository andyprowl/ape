cmake_minimum_required(VERSION 3.10)

project(QtEngine)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")

    set(CMAKE_INCLUDE_CURRENT_DIR ON)

endif()

# This is necessary so that mocs_compilation.cpp is properly auto-generated
find_package(Qt5 COMPONENTS OpenGL Widgets REQUIRED)

AddLibrary(${PROJECT_NAME})

AddPublicDependencies(${PROJECT_NAME} Engine Glad Qt5::OpenGL Qt5::Widgets Qt5::Core)

AddPrivateDependencies(${PROJECT_NAME} opengl32)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
