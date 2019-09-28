cmake_minimum_required(VERSION 3.10)

project(QtWindow)

set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")

    set(CMAKE_INCLUDE_CURRENT_DIR ON)

endif()

find_package(Qt5 COMPONENTS OpenGL Widgets REQUIRED)

AddLibrary(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC Core Qt5::OpenGL Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
